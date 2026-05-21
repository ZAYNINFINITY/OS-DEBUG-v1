# GUI Prototype (Milestone 1)

This folder contains the first GUI implementation for `OS Fault Detective`.

## What is implemented

- Matrix-style animated background
- Main menu with mission-console visual style
- Chapter select screen (all 8 cases playable)
- Command-driven debugging flow for each case
- Outcome scoring and persistent run stats panel

## How to run

From this `gui` directory:

```bash
python server.py
```

Then open:

`http://localhost:8080`

This uses the GUI static server plus the JSON API (`/api/env`, `/api/exec`) needed for the integrated terminal.

## Next GUI milestones

- Connect all 8 cases
- Persist run stats to file/backend
- Reuse CLI core logic model in a shared engine layer
- Add richer animations/sound and chapter map
