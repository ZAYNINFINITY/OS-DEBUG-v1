const screens = {
  menu: document.getElementById("screen-menu"),
  briefing: document.getElementById("screen-briefing"),
  chapters: document.getElementById("screen-chapters"),
  case1: document.getElementById("screen-case1"),
  about: document.getElementById("screen-about"),
  shell: document.getElementById("screen-shell"),
  stats: document.getElementById("screen-stats"),
};

const CASES = {
  1: {
    title: "Case 1: The Frozen City",
    concept: "Deadlock",
    timerSecs: 180,
    objectives: [
      "Collect 2 evidence sources",
      "Diagnose deadlock correctly",
      "Break circular wait safely",
    ],
    tools: ["inspect", "logs", "graph"],
    briefingPages: [
      "03:47 AM. NovaCorp traffic control is stalled. Queue processors are alive but no transaction completes.",
      "Flashback: P1 holds A and requests B, P2 holds B and requests C, P3 holds C and requests A. Circular wait has locked progress.",
      "Objective: confirm deadlock conditions, produce correct diagnosis, and break the wait cycle with minimal collateral impact.",
    ],
    evidenceCommands: {
      inspect: "Inspector: P1 waits B, P2 waits C, P3 waits A.",
      logs: "Logs: all 3 processes blocked for 6 hours.",
      graph: "Graph cycle: P1->P2->P3->P1",
    },
    needEvidence: 2,
    diagnose: "diagnose deadlock",
    diagnoseHint: "Try diagnose deadlock | diagnose starvation | diagnose memory",
    fix: "fix terminate p3",
    fixHint: "Try fix terminate p3 | fix priority | fix clear-cache",
  },
  2: {
    title: "Case 2: The Ignored Worker",
    concept: "Starvation",
    timerSecs: 180,
    objectives: ["Collect evidence", "Diagnose starvation", "Apply aging fix"],
    tools: ["inspect", "logs", "timeline"],
    briefingPages: [
      "A critical reporting worker has been queued but never scheduled.",
      "Incoming high-priority bursts keep preempting lower-priority work, causing indefinite wait.",
      "Objective: prove starvation, then restore fairness with aging while preserving throughput.",
    ],
    evidenceCommands: {
      inspect: "Low-priority process DELTA never gets CPU time.",
      logs: "Queue trace: high-priority tasks keep arriving.",
      timeline: "Gantt timeline: DELTA wait time keeps growing.",
    },
    needEvidence: 2,
    diagnose: "diagnose starvation",
    diagnoseHint: "Try diagnose starvation | diagnose deadlock",
    fix: "fix aging",
    fixHint: "Try fix aging | fix round-robin-only",
  },
  3: {
    title: "Case 3: The Memory Thief",
    concept: "Memory Management",
    timerSecs: 180,
    objectives: ["Inspect memory evidence", "Diagnose leak", "Reclaim and compact"],
    tools: ["inspect", "logs", "map"],
    briefingPages: [
      "Memory pressure is rising. Available pages are critically low and reclaim is not recovering capacity.",
      "One service exceeds expected allocation, indicating leak-like growth and fragmentation side effects.",
      "Objective: diagnose memory management failure and execute reclaim + compaction strategy.",
    ],
    evidenceCommands: {
      inspect: "Process P4 exceeded expected page allocation.",
      logs: "Leak warnings repeated, reclaim never triggered.",
      map: "Memory map: 9/10 pages occupied by one process.",
    },
    needEvidence: 2,
    diagnose: "diagnose memory",
    diagnoseHint: "Try diagnose memory | diagnose deadlock",
    fix: "fix reclaim compact",
    fixHint: "Try fix reclaim compact | fix restart-all",
  },
  4: {
    title: "Case 4: Root Cause Boss",
    concept: "Combined Core",
    timerSecs: 240,
    objectives: ["Gather multi-system evidence", "Diagnose root cause", "Apply staged recovery"],
    tools: ["inspect", "logs", "trace"],
    briefingPages: [
      "Multiple subsystems are failing concurrently under production load.",
      "Symptoms overlap across scheduling fairness, resource waiting, and memory saturation.",
      "Objective: perform structured triage and staged mitigation rather than single-point guessing.",
    ],
    evidenceCommands: {
      inspect: "Mixed symptoms: waiting chain + queue unfairness + pressure spike.",
      logs: "Multiple subsystem alerts firing together.",
      trace: "Trace shows cascading dependency failures.",
    },
    needEvidence: 2,
    diagnose: "diagnose root-cause",
    diagnoseHint: "Try diagnose root-cause | diagnose single-bug",
    fix: "fix round-robin",
    fixHint: "Try fix round-robin | fix sjf",
  },
  5: {
    title: "Case 5: Syscall Maze",
    concept: "fork/exec/wait/strace",
    timerSecs: 180,
    objectives: ["Trace syscall path", "Find parent-child issue", "Fix exec/wait handling"],
    tools: ["strace", "inspect", "logs"],
    briefingPages: [
      "Worker manager forks child processes, but execution success is inconsistent.",
      "Trace indicates failed exec path and incomplete parent wait/reap handling.",
      "Objective: validate fork/exec/wait flow using syscall-level evidence (`strace`).",
    ],
    evidenceCommands: {
      strace: "strace: execve fails with ENOENT in child path.",
      inspect: "Zombie children observed in process table.",
      logs: "wait status mismatch after child launch.",
    },
    needEvidence: 2,
    diagnose: "diagnose syscall-path",
    diagnoseHint: "Try diagnose syscall-path | diagnose scheduling",
    fix: "fix validate-exec-wait",
    fixHint: "Try fix validate-exec-wait | fix remove-wait",
  },
  6: {
    title: "Case 6: Lockdown",
    concept: "Synchronization",
    timerSecs: 180,
    objectives: ["Capture race evidence", "Diagnose synchronization bug", "Apply mutex/semaphore fix"],
    tools: ["inspect", "logs", "probe"],
    briefingPages: [
      "Shared state diverges under concurrency spikes and write races are visible.",
      "Critical section violations indicate missing or misapplied synchronization policy.",
      "Objective: diagnose race condition and apply mutex/semaphore strategy (Peterson discussed for 2-process baseline).",
    ],
    evidenceCommands: {
      inspect: "Shared counter diverges across threads.",
      logs: "Critical section collisions detected.",
      probe: "Lock contention spikes without proper guards.",
    },
    needEvidence: 2,
    diagnose: "diagnose race",
    diagnoseHint: "Try diagnose race | diagnose memory",
    fix: "fix mutex",
    fixHint: "Try fix mutex | fix busy-wait",
  },
  7: {
    title: "Case 7: The Orphan Forest",
    concept: "Process Lifecycle",
    timerSecs: 180,
    objectives: ["Inspect process tree", "Diagnose lifecycle issue", "Apply waitpid strategy"],
    tools: ["inspect", "logs", "tree"],
    briefingPages: [
      "Zombie count increases over time and process table pressure rises.",
      "Parent processes are not reliably reaping exited children; orphan branches accumulate.",
      "Objective: restore parent-child lifecycle control and healthy process-state transitions.",
    ],
    evidenceCommands: {
      inspect: "Parent processes not reaping exited children.",
      logs: "Zombie count increasing steadily.",
      tree: "Process tree shows orphaned branches.",
    },
    needEvidence: 2,
    diagnose: "diagnose zombie",
    diagnoseHint: "Try diagnose zombie | diagnose deadlock",
    fix: "fix waitpid-timeout",
    fixHint: "Try fix waitpid-timeout | fix spawn-more",
  },
  8: {
    title: "Case 8: Broken Pipeline",
    concept: "Client-Server + IPC",
    timerSecs: 180,
    objectives: ["Gather queue/pipe evidence", "Diagnose pipeline failure", "Apply backpressure fix"],
    tools: ["inspect", "logs", "trace"],
    briefingPages: [
      "Client retries are saturating server queues and latency SLO is breached.",
      "Pipeline stages are misaligned, producing blocked reads and dropped responses.",
      "Objective: diagnose IPC/service flow failure and recover with bounded queues + backpressure.",
    ],
    evidenceCommands: {
      inspect: "Client retries saturate server queue.",
      logs: "Pipe writer closes unexpectedly.",
      trace: "Pipeline stage B waiting on empty input.",
    },
    needEvidence: 2,
    diagnose: "diagnose pipeline",
    diagnoseHint: "Try diagnose pipeline | diagnose scheduling",
    fix: "fix backpressure-bounded-queue",
    fixHint: "Try fix backpressure-bounded-queue | fix infinite-workers",
  },
};

const state = {
  currentCaseId: 1,
  campaignMode: false,
  wrongMoves: 0,
  stability: 100,
  step: 1,
  runLog: JSON.parse(localStorage.getItem("osfd_gui_runlog") || "[]"),
  evidence: {},
  timerSecsLeft: 0,
  timerId: null,
  briefingPageIdx: 0,
  typewriterId: null,
};

function showScreen(name) {
  Object.values(screens).forEach((s) => s.classList.remove("active"));
  screens[name].classList.add("active");
}

function currentCase() {
  return CASES[state.currentCaseId];
}

function terminalLog(msg) {
  const log = document.getElementById("case1-terminal-log");
  const p = document.createElement("p");
  p.innerHTML = msg;
  log.appendChild(p);
  log.scrollTop = log.scrollHeight;
}

function resetTerminal() {
  const log = document.getElementById("case1-terminal-log");
  log.innerHTML = "";
  terminalLog(`Mission started: <b>${currentCase().title}</b>. Type <code>help</code>.`);
}

function penalty(msg) {
  state.wrongMoves += 1;
  state.stability = Math.max(0, state.stability - 15);
  terminalLog(`<span class="danger">${msg} (stability ${state.stability}%)</span>`);
}

function updateHud() {
  const evidenceSummary = Object.keys(state.evidence)
    .map((k) => `${k}=${state.evidence[k] ? "yes" : "no"}`)
    .join(", ");
  document.getElementById("case1-hud").innerHTML = `
    <p><b>Stability:</b> ${state.stability}% | <b>Wrong moves:</b> ${state.wrongMoves} | <b>Timer:</b> ${state.timerSecsLeft}s</p>
    <p><b>Concept:</b> ${currentCase().concept} | <b>Evidence:</b> ${evidenceSummary}</p>
    <p><b>Tools:</b> ${currentCase().tools.join(", ")}</p>
    <p><b>Objectives:</b> ${currentCase().objectives.join(" -> ")}</p>
  `;
}

function resetCase(caseId) {
  state.currentCaseId = caseId;
  state.wrongMoves = 0;
  state.stability = 100;
  state.step = 1;
  state.timerSecsLeft = currentCase().timerSecs;
  state.evidence = {};
  Object.keys(currentCase().evidenceCommands).forEach((k) => (state.evidence[k] = false));
  document.getElementById("case-title").textContent = currentCase().title;
  resetTerminal();
  startTimer();
  renderCase();
}

function nextCampaignCaseId() {
  const nextId = state.currentCaseId + 1;
  return CASES[nextId] ? nextId : null;
}

function evidenceCount() {
  return Object.values(state.evidence).filter(Boolean).length;
}

function renderCase() {
  const content = document.getElementById("case1-content");
  const actions = document.getElementById("case1-actions");
  const stepLabel = document.getElementById("case1-step-label");
  const c = currentCase();
  updateHud();

  if (state.stability <= 0) {
    stepLabel.textContent = "Crashed";
    content.innerHTML = `<p class="danger">System crashed due to wrong interventions.</p>`;
    actions.innerHTML = `<p>Type <code>finish fail</code> to record this run.</p>`;
    return;
  }

  if (state.step === 1) {
    stepLabel.textContent = "Step 1/3 - Investigate";
    const list = Object.keys(c.evidenceCommands).map((k) => `<code>${k}</code>`).join(", ");
    content.innerHTML = `<p>Collect independent evidence before committing a diagnosis.</p>`;
    actions.innerHTML = `<p>Use evidence commands: ${list}</p><p>Requirement: ${c.needEvidence} valid sources before diagnosis step unlocks.</p>`;
    return;
  }
  if (state.step === 2) {
    stepLabel.textContent = "Step 2/3 - Diagnose";
    content.innerHTML = `<p>Submit the root-cause diagnosis based on evidence.</p>`;
    actions.innerHTML = `<p>${c.diagnoseHint}</p>`;
    return;
  }
  stepLabel.textContent = "Step 3/3 - Resolve";
  content.innerHTML = `<p>Apply mitigation command and validate recovery.</p>`;
  actions.innerHTML = `<p>${c.fixHint}</p>`;
}

function finishCase(success) {
  stopTimer();
  const ending = success ? (state.wrongMoves === 0 ? "Perfect" : "Survived") : "Crashed";
  const score = Math.max(0, (success ? 1000 : 0) - state.wrongMoves * 150);
  const entry = {
    at: new Date().toLocaleString(),
    caseId: state.currentCaseId,
    caseTitle: currentCase().title,
    concept: currentCase().concept,
    ending,
    score,
    stability: state.stability,
    wrongMoves: state.wrongMoves,
  };
  state.runLog.push(entry);
  localStorage.setItem("osfd_gui_runlog", JSON.stringify(state.runLog));
  renderStats(entry);
  const replayBtn = document.getElementById("btn-restart-case");
  if (state.campaignMode && success && nextCampaignCaseId()) {
    replayBtn.textContent = `Continue Campaign -> Case ${nextCampaignCaseId()}`;
  } else {
    replayBtn.textContent = "Replay Current Case";
  }
  showScreen("stats");
}

function startTimer() {
  stopTimer();
  state.timerId = setInterval(() => {
    state.timerSecsLeft = Math.max(0, state.timerSecsLeft - 1);
    if (screens.case1.classList.contains("active")) renderCase();
    if (state.timerSecsLeft <= 0) {
      stopTimer();
      terminalLog("<span class='danger'>Timer expired. Mission failed.</span>");
      finishCase(false);
    }
  }, 1000);
}

function stopTimer() {
  if (state.timerId) clearInterval(state.timerId);
  state.timerId = null;
}

function renderStats(latest) {
  const stats = document.getElementById("stats-output");
  const rows = state.runLog
    .slice()
    .reverse()
    .map(
      (r, i) =>
        `<p>#${i + 1} [Case ${r.caseId}] ${r.caseTitle} | ${r.at} | Ending: <b>${r.ending}</b> | Score: ${r.score} | Stability: ${r.stability}% | Wrong: ${r.wrongMoves}</p>`
    )
    .join("");
  const avg = state.runLog.length ? Math.round(state.runLog.reduce((a, b) => a + b.score, 0) / state.runLog.length) : 0;
  stats.innerHTML = `
    <p><b>Latest Result:</b> ${latest.caseTitle} -> ${latest.ending} (Score ${latest.score})</p>
    <p><b>Total runs:</b> ${state.runLog.length} | <b>Average score:</b> ${avg}</p>
    <p><b>Persistent stats:</b> stored in browser localStorage.</p>
    <hr />
    ${rows || "<p>No runs yet.</p>"}
  `;
}

function handleCommand(raw) {
  const rawCmd = raw.trim().toLowerCase();
  const aliases = {
    ps: "inspect",
    log: "logs",
    graphview: "graph",
    diag: "diagnose",
    solve: "fix",
    objective: "objectives",
    tool: "tools",
    cls: "clear",
  };
  let cmd = rawCmd;
  const first = rawCmd.split(" ")[0];
  if (aliases[first]) {
    cmd = aliases[first] + (rawCmd.length > first.length ? rawCmd.slice(first.length) : "");
  }
  if (!cmd) return;
  const c = currentCase();
  terminalLog(`&gt; ${cmd}`);

  if (cmd === "help") {
    terminalLog(`Evidence commands: ${Object.keys(c.evidenceCommands).join(", ")}`);
    terminalLog(`Diagnosis command: ${c.diagnose}`);
    terminalLog(`Fix command: ${c.fix}`);
    terminalLog("General commands: status, tools, objectives, hint, clear, finish fail");
    return;
  }
  if (cmd === "clear") {
    document.getElementById("case1-terminal-log").innerHTML = "";
    terminalLog("Console cleared.");
    return;
  }
  if (cmd === "status") {
    terminalLog(`Step ${state.step}/3 | Stability ${state.stability}% | Evidence ${evidenceCount()}/${c.needEvidence}`);
    return;
  }
  if (cmd === "tools") {
    terminalLog(`Available tools: ${c.tools.join(", ")}`);
    return;
  }
  if (cmd === "objectives") {
    terminalLog(`Objectives: ${c.objectives.join(" -> ")}`);
    return;
  }
  if (cmd === "hint") {
    if (state.step === 1) terminalLog(`Collect at least ${c.needEvidence} evidence sources.`);
    else if (state.step === 2) terminalLog(`Use: ${c.diagnose}`);
    else terminalLog(`Use: ${c.fix}`);
    return;
  }
  if (cmd in c.evidenceCommands) {
    state.evidence[cmd] = true;
    terminalLog(c.evidenceCommands[cmd]);
  } else if (cmd.startsWith("diagnose ")) {
    if (state.step < 2) {
      terminalLog("Collect evidence first.");
      return;
    }
    if (cmd === c.diagnose) {
      terminalLog("<span class='good'>Diagnosis accepted.</span>");
      state.step = 3;
    } else {
      penalty("Wrong diagnosis");
    }
  } else if (cmd.startsWith("fix ")) {
    if (state.step < 3) {
      terminalLog("Diagnose first.");
      return;
    }
    if (cmd === c.fix) {
      terminalLog("<span class='good'>Fix applied successfully.</span>");
      finishCase(true);
      return;
    }
    penalty("Fix failed");
  } else if (cmd === "finish fail") {
    finishCase(false);
    return;
  } else {
    penalty("Unknown command");
  }

  if (state.step === 1 && evidenceCount() >= c.needEvidence) {
    state.step = 2;
    terminalLog("<span class='good'>Enough evidence. Move to diagnosis.</span>");
  }
  renderCase();
}

function shellLog(msg) {
  const log = document.getElementById("shell-log");
  const p = document.createElement("p");
  p.innerHTML = msg;
  log.appendChild(p);
  log.scrollTop = log.scrollHeight;
}

async function refreshShellEnv() {
  try {
    const data = await (await fetch("/api/env")).json();
    const root = data.default_cwd || data.project_root || "";
    const pl = document.getElementById("shell-prompt-label");
    if (pl) pl.textContent = data.is_windows ? "ps@osfd" : "ubuntu@osfd";
    const cwdEl = document.getElementById("shell-cwd-label");
    if (cwdEl && root) cwdEl.textContent = `cwd: ${root}`;
    const shell = data.shell || (data.is_windows ? "powershell" : "bash");
    const mode = data.terminal_mode || (data.is_windows ? "powershell" : "ubuntu");
    shellLog(`<span class="good">Terminal ready</span> | ${shell} | ${mode}`);
    shellLog(`Try: pwd, ls, cd osfault, cat README.md, make -C osfault`);
  } catch {
    shellLog("<span class='danger'>Failed to load environment</span>");
  }
}

async function runShellCommand(cmd) {
  if (!cmd.trim()) return;
  shellLog(`<span class="prompt">ubuntu@osfd:~$</span> ${escapeHtml(cmd)}`);
  try {
    const res = await fetch("/api/exec", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ command: cmd, session: "gui-shell" }),
    });
    const data = await res.json();
    if (data.stdout) {
      shellLog(`<pre>${escapeHtml(data.stdout)}</pre>`);
    }
    if (data.stderr) {
      shellLog(`<pre class="danger">${escapeHtml(data.stderr)}</pre>`);
    }
    if (data.cwd) {
      document.getElementById("shell-cwd-label").textContent = `cwd: ${data.cwd}`;
    }
    // Show exit code if non-zero
    if (data.exit_code !== 0 && data.exit_code !== undefined) {
      shellLog(`<span class="danger">exit code: ${data.exit_code}</span>`);
    }
  } catch (e) {
    shellLog(`<span class="danger">Terminal backend not reachable. Start GUI via: python gui/server.py</span>`);
  }
}

function escapeHtml(s) {
  return s
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;");
}

function typewriterTo(el, text, speed = 16, done) {
  if (state.typewriterId) {
    clearInterval(state.typewriterId);
    state.typewriterId = null;
  }
  el.textContent = "";
  let i = 0;
  state.typewriterId = setInterval(() => {
    el.textContent += text[i] || "";
    i++;
    if (i > text.length) {
      clearInterval(state.typewriterId);
      state.typewriterId = null;
      if (done) done();
    }
  }, speed);
}

function showBriefing(caseId, campaignMode = false) {
  state.currentCaseId = caseId;
  state.campaignMode = campaignMode;
  state.briefingPageIdx = 0;
  document.getElementById("briefing-title").textContent = `${currentCase().title} - Briefing`;
  showScreen("briefing");
  renderBriefingPage();
}

function renderBriefingPage() {
  const pages = currentCase().briefingPages || [];
  const page = state.briefingPageIdx;
  const textEl = document.getElementById("briefing-text");
  document.getElementById("briefing-page").textContent = `Page ${page + 1}/${pages.length}`;
  typewriterTo(textEl, pages[page], 14);
  document.getElementById("btn-briefing-next").textContent =
    page >= pages.length - 1 ? "Start Working" : "Next Page";
}

(function matrixBg() {
  const canvas = document.getElementById("matrix");
  const ctx = canvas.getContext("2d");
  let w = (canvas.width = window.innerWidth);
  let h = (canvas.height = window.innerHeight);
  const cols = Math.floor(w / 16);
  const drops = new Array(cols).fill(1);

  function draw() {
    ctx.fillStyle = "rgba(0, 0, 0, 0.08)";
    ctx.fillRect(0, 0, w, h);
    ctx.fillStyle = "#30ff88";
    ctx.font = "14px monospace";
    for (let i = 0; i < drops.length; i++) {
      const text = String(Math.floor(Math.random() * 10));
      ctx.fillText(text, i * 16, drops[i] * 16);
      if (drops[i] * 16 > h && Math.random() > 0.975) drops[i] = 0;
      drops[i]++;
    }
  }

  setInterval(draw, 50);
  window.addEventListener("resize", () => {
    w = canvas.width = window.innerWidth;
    h = canvas.height = window.innerHeight;
  });
})();

function bindClick(id, handler) {
  const el = document.getElementById(id);
  if (el) el.onclick = handler;
}

bindClick("btn-play", () => {
  showBriefing(1, true);
});
bindClick("btn-chapters", () => showScreen("chapters"));
bindClick("btn-tutorial", () => showScreen("about"));
bindClick("btn-shell", () => {
  showScreen("shell");
  refreshShellEnv();
  shellLog("<b>Integrated Terminal</b> - runs commands on your local machine via the GUI server.");
  shellLog("Type commands like: pwd, ls, cd, cat, make. Use 'help' or 'man <command>' when available.");
});
bindClick("btn-menu-stats", () => {
  renderStats(state.runLog[state.runLog.length - 1] || { caseTitle: "-", ending: "-", score: 0 });
  showScreen("stats");
});
bindClick("btn-back-menu-1", () => {
  stopTimer();
  state.campaignMode = false;
  showScreen("menu");
});
bindClick("btn-back-menu-2", () => {
  stopTimer();
  state.campaignMode = false;
  showScreen("menu");
});
bindClick("btn-back-menu-3", () => showScreen("menu"));
bindClick("btn-back-menu-4", () => showScreen("menu"));
bindClick("btn-back-menu-6", () => showScreen("menu"));
bindClick("btn-briefing-next", () => {
  const pages = currentCase().briefingPages || [];
  if (state.briefingPageIdx >= pages.length - 1) {
    resetCase(state.currentCaseId);
    showScreen("case1");
    return;
  }
  state.briefingPageIdx += 1;
  renderBriefingPage();
});
bindClick("btn-briefing-skip", () => {
  resetCase(state.currentCaseId);
  showScreen("case1");
});
bindClick("btn-restart-case", () => {
  const nextId = state.campaignMode ? nextCampaignCaseId() : null;
  if (nextId) {
    showBriefing(nextId, true);
    return;
  }
  resetCase(state.currentCaseId);
  showScreen("case1");
});
bindClick("case1-terminal-send", () => {
  const input = document.getElementById("case1-terminal-input");
  handleCommand(input.value);
  input.value = "";
});
document.getElementById("case1-terminal-input").addEventListener("keydown", (e) => {
  if (e.key === "Enter") {
    document.getElementById("case1-terminal-send").click();
  }
});
document.querySelectorAll(".chapter.playable").forEach((btn) => {
  btn.onclick = () => {
    showBriefing(parseInt(btn.dataset.chapter, 10), false);
  };
});

bindClick("shell-send", () => {
  const input = document.getElementById("shell-input");
  const cmd = input.value.trim();
  if (!cmd) return;
  runShellCommand(cmd);
  input.value = "";
  input.focus();
});
document.getElementById("shell-input").addEventListener("keydown", (e) => {
  if (e.key === "Enter") document.getElementById("shell-send").click();
});
bindClick("btn-shell-help", () => {
  shellLog("<b>=== Useful Commands ===</b>");
  shellLog("<b>Navigation:</b> pwd, ls, ls -la, cd dir, cd ..");
  shellLog("<b>Files:</b> cat file, echo text, touch file, rm file");
  shellLog("<b>Project:</b> make -C osfault, ./scripts/ubuntu/run_cli.sh");
  shellLog("<b>Process:</b> ps aux, top, kill PID");
  shellLog("This terminal follows your local shell. Type 'help' in CLI mode for more.");
});
