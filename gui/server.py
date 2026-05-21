#!/usr/bin/env python3
"""Static GUI + JSON API: /api/env, /api/exec.

The GUI includes an "integrated terminal". This server keeps the GUI static files and
provides a small JSON API so the frontend can run a limited set of local commands.
"""
import json
import os
import shutil
import subprocess
import sys
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
GUI_DIR = os.path.dirname(os.path.abspath(__file__))
OSFAULT_DIR = os.path.join(PROJECT_ROOT, "osfault")

_session_cwd = {}

IS_WINDOWS = os.name == "nt"


def _session_get(body):
    sid = (body.get("session") or "").strip()
    if not sid:
        sid = "default"
    if sid not in _session_cwd:
        _session_cwd[sid] = OSFAULT_DIR if os.path.isdir(OSFAULT_DIR) else PROJECT_ROOT
    return sid


def _run_shell_command(cmd, cwd):
    """Execute a command in the appropriate local shell (Windows PowerShell / Linux bash)."""
    if IS_WINDOWS:
        shell = shutil.which("pwsh") or shutil.which("powershell") or "powershell"
        return subprocess.run(
            [shell, "-NoProfile", "-NonInteractive", "-Command", cmd],
            cwd=cwd,
            text=True,
            capture_output=True,
            timeout=45,
            env=os.environ.copy(),
        )

    bash = "/bin/bash" if os.path.isfile("/bin/bash") else (shutil.which("bash") or "bash")
    env = os.environ.copy()
    env.setdefault("TERM", "xterm-256color")
    env.setdefault("LANG", "C.UTF-8")
    env["SHELL"] = bash
    env["PS1"] = "ubuntu@osfd:${PWD##*/}$ "
    return subprocess.run(
        [bash, "-c", cmd],
        cwd=cwd,
        text=True,
        capture_output=True,
        timeout=45,
        env=env,
    )


def _is_blocked_command(cmd: str) -> bool:
    cmd_l = cmd.lower()
    blocked_common = [
        "\x00",  # Null byte injection
        ": (){",  # Fork bomb
        "mkfs",
        "dd if=",
    ]

    if IS_WINDOWS:
        blocked_windows = [
            "del ",
            "erase ",
            "rd ",
            "rmdir",
            "format ",
            "diskpart",
            "shutdown ",
            "stop-computer",
            "restart-computer",
            "remove-item",
            "clear-content",
            "set-content",
        ]
        return any(b in cmd_l for b in blocked_common + blocked_windows)

    blocked_linux = [
        "rm -rf",
        "poweroff",
        "shutdown ",
        # Windows-specific commands (terminal is Linux)
        "cmd.exe",
        "powershell",
        "diskpart",
        "format ",
        "net user",
        "taskkill",
        "systeminfo",
        "tasklist",
    ]
    return any(b in cmd_l for b in blocked_common + blocked_linux)


class Handler(SimpleHTTPRequestHandler):
    def _send_json(self, code, payload):
        data = json.dumps(payload).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.path == "/api/env":
            if IS_WINDOWS:
                shell = "pwsh" if shutil.which("pwsh") else "powershell"
            else:
                shell = "bash"
            self._send_json(
                200,
                {
                    "platform": "windows" if IS_WINDOWS else "linux",
                    "project_root": PROJECT_ROOT,
                    "default_cwd": OSFAULT_DIR if os.path.isdir(OSFAULT_DIR) else PROJECT_ROOT,
                    "is_windows": IS_WINDOWS,
                    "terminal_mode": "powershell" if IS_WINDOWS else "ubuntu",
                    "shell": shell,
                },
            )
            return
        return SimpleHTTPRequestHandler.do_GET(self)

    def do_POST(self):
        if self.path != "/api/exec":
            self._send_json(404, {"error": "not_found"})
            return
        body = {}
        sid = "default"
        cwd = PROJECT_ROOT
        try:
            length = int(self.headers.get("Content-Length", "0"))
            raw = self.rfile.read(length).decode("utf-8")
            body = json.loads(raw or "{}")
            cmd = (body.get("command") or "").strip()
            sid = _session_get(body)
            cwd = _session_cwd[sid]

            if not cmd:
                self._send_json(400, {"error": "empty_command"})
                return

            if cmd.startswith("cd "):
                target = cmd[3:].strip().strip('"').strip("'")
                if target in ("~", "$HOME"):
                    target = os.path.expanduser("~")
                elif target == "-":
                    self._send_json(
                        200,
                        {"stdout": "", "stderr": "cd: - not supported in web terminal\n", "cwd": cwd, "exit_code": 1, "session": sid},
                    )
                    return
                elif not os.path.isabs(target):
                    target = os.path.abspath(os.path.join(cwd, target))
                if os.path.isdir(target):
                    _session_cwd[sid] = target
                    self._send_json(
                        200,
                        {"stdout": "", "stderr": "", "cwd": _session_cwd[sid], "exit_code": 0, "session": sid},
                    )
                else:
                    self._send_json(
                        200,
                        {
                            "stdout": "",
                            "stderr": f"cd: no such directory: {target}\n",
                            "cwd": cwd,
                            "exit_code": 1,
                            "session": sid,
                        },
                    )
                return

            if _is_blocked_command(cmd):
                self._send_json(
                    200,
                    {
                        "stdout": "",
                        "stderr": "Command blocked (dangerous for integrated terminal).\n",
                        "cwd": cwd,
                        "exit_code": 1,
                        "session": sid,
                    },
                )
                return

            if cmd.startswith("osfault") or cmd.startswith("./osfault") or cmd.startswith(".\\osfault"):
                if not cmd.startswith("./") and not cmd.startswith(".\\"):
                    if IS_WINDOWS:
                        cmd = ".\\osfault.exe " + cmd[8:] if cmd.startswith("osfault") else cmd
                    else:
                        cmd = "./osfault " + cmd[8:] if cmd.startswith("osfault") else cmd

            completed = _run_shell_command(cmd, cwd)
            self._send_json(
                200,
                {
                    "stdout": completed.stdout or "",
                    "stderr": completed.stderr or "",
                    "cwd": cwd,
                    "exit_code": completed.returncode,
                    "session": sid,
                },
            )
        except subprocess.TimeoutExpired:
            cw = _session_cwd.get(sid, PROJECT_ROOT)
            self._send_json(
                200,
                {"stdout": "", "stderr": "Command timed out (45s).\n", "cwd": cw, "exit_code": 124, "session": sid},
            )
        except Exception as e:
            self._send_json(500, {"error": str(e)})

def main():
    os.chdir(GUI_DIR)
    port = int(os.environ.get("PORT", "8080"))
    server = ThreadingHTTPServer(("127.0.0.1", port), Handler)
    print(f"http://127.0.0.1:{port}/")
    server.serve_forever()


if __name__ == "__main__":
    main()
