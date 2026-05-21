# OS-DEBUG-v1 — OS Fault Detective

**CLI BASED + GUI prototype** for an educational “operational incident simulator” game.

- **GUI Prototype (Milestone 1)** lives in `gui/`.
- The GUI includes an **animated matrix background**, **mission/briefing flows**, **case/chapter selection (8 cases)**, and an **integrated web terminal** backed by a small JSON API.

---

## Project structure

```text
OS-DEBUG-v1/
  README.md
  CLI  ( main )/         # CLI code (not documented here yet)
  gui/
    app.js               # Frontend logic (screens, cases, terminal UI, animations)
    index.html           # Frontend layout (screens + terminal panels)
    styles.css           # Retro CRT styling + animations
    server.py            # Python JSON API server (serves UI + /api/env + /api/exec)
    README.md           # GUI notes (older / shorter)
```

---

## GUI Prototype (Milestone 1)

### What you get

- **Matrix-style animated background** (canvas)
- **Typewriter briefing animation** (page-by-page text reveal)
- **CRT/scanline vibe styling** (neon glow + retro panels)
- **Timer-driven mission flow** (HUD step progression + countdown)

- **Main menu**: Start Campaign / How to play / Chapter Select / Run Stats / Integrated Terminal
- **Briefing with typewriter ** (page-by-page)
- **Chapter Select**: **8 playable cases**
- **Mission Console**: commaeffectnd-driven debugging flow
  - Collect evidence → Diagnose → Resolve
  - Stability decreases on wrong diagnosis/fix
- **Run Stats**: results persisted in **browser `localStorage`**
- **Integrated Terminal**:
  - Runs local commands through `gui/server.py`
  - Includes basic command blocking for safety
  - Uses a persistent working directory per session

---

## Run the GUI

From the `gui` folder:

```bash
python server.py
```

Then open:

- http://localhost:8080

> The server binds to **127.0.0.1** and uses port **8080** by default (override with `PORT`).

---

## How the Mission Console works (in the GUI)

Each case is a mini debugging mission. Commands are typed into the **Mission Console** for that case.

### Core commands

- `help` — show available commands
- `status` — show step, stability, and evidence progress
- `tools` — show evidence tools for the current case
- `objectives` — show case objectives
- `hint` — show context-sensitive hint
- `clear` — clear the Mission Console

### Evidence commands

Use **case-specific** evidence commands (examples vary by case). Once you collect enough evidence (`needEvidence`), the mission unlocks the diagnosis step.

### Diagnosis + Fix format

- Diagnosis must match the case root cause, e.g.:
  - `diagnose deadlock`
- Fix must match the case mitigation, e.g.:
  - `fix terminate p3`

If you submit the wrong diagnosis or wrong fix, the mission applies a **stability penalty**.

---

## Integrated Terminal (web → local command runner)

The GUI also includes a terminal panel that runs commands on your machine via the backend API:

- `GET /api/env` — returns platform + default cwd + shell info
- `POST /api/exec` — executes a command with a session-aware working directory

### Safety / blocking

Dangerous commands are blocked (fork bombs, destructive disk operations, shutdown/format-like commands, etc.).

### Example workflow

In the GUI terminal try commands like:

- `pwd`
- `ls`
- `cat README.md`
- `make -C osfault`

The backend also supports `cd <path>` and keeps the current directory per session.

---

## Animated UI details

- **Background:** `gui/app.js` renders the animated matrix-like canvas.
- **Briefing text:** typewriter effect for immersion (`typewriterTo`).
- **Retro styling:** `gui/styles.css` uses neon colors, CRT-like panels, and hover effects.

---

## Run stats

After each case run, the GUI logs an entry:

- case id/title/concept
- ending (Perfect/Survived/Crashed)
- score
- stability
- wrong move count

These are stored in browser `localStorage` under:

- `osfd_gui_runlog`

---

## Notes / Milestones

- GUI 8 cases are implemented as a structured object in `gui/app.js`.
- The CLI exists in `CLI  ( main )/` but is not covered by this README yet.
- Next steps (based on existing GUI README):
  - unify CLI core logic with the GUI
  - persist stats beyond localStorage
  - improve animations/sound and add more UX

---

## Quick start

```bash
cd gui
python server.py
```

Open http://localhost:8080
