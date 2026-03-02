# 123Pomadoro — Pomodoro Timer for Flipper Zero

Classic Pomodoro timer for focused work. Run focus sessions, take short breaks, and finish with a long break, with vibration and sound cues on each phase change.

Current version: `1.1.3`

## Features

- **Classic Pomodoro flow** — focus sessions, short breaks, final long break
- **Configurable timers** — focus, short break, long break, and round count
- **Pause/Resume** with OK
- **Reset Settings** action in settings
- **Persistent settings** — saved on SD card
- **Haptic and sound alerts** for phase transitions
- **Lightweight session animations** for focus and break states
- **Tomato app icon** in the Flipper app list

## Screenshots

```
┌────────────────────────────┐   ┌────────────────────────────┐
│      123Pomadoro           │   │ Settings                   │
│                            │   │ Focus               < 25m >│
│  > Start Cycle             │   │ Short Break          < 5m >│
│    Settings                │   │ Long Break          < 15m >│
│    About                   │   │ Rounds                < 4 >│
│                            │   │ Reset Settings    < Reset> │
│                            │   │                            │
└────────────────────────────┘   └────────────────────────────┘
         Main Menu                         Settings

┌────────────────────────────┐   ┌────────────────────────────┐
│ 123Pomadoro        12:40   │   │                            │
│                            │   │                            │
│          Focus             │   │          Cycle             │
│        Round 2 / 4         │   │         Complete!          │
│          18:12             │   │                            │
│    █                  █    │   │           [OK]             │
│   [██████████░░░░░░░]      │   │                            │
└────────────────────────────┘   └────────────────────────────┘
       Session Screen                    Done Screen
```

## Install

### From source

```bash
ufbt
ufbt launch
```

### On Flipper

- App file: `/ext/apps/Tools/pomadoro.fap`
- Settings folder: `/ext/apps_data/pomadoro/`

## Usage

1. Open `Applications -> Tools -> 123Pomadoro`
2. Adjust timers in `Settings` if needed
3. Start `Start Cycle`
4. Use `OK` to pause or resume
5. Use `Back` to exit the active cycle

## Controls

| Button | Action |
|--------|--------|
| OK | Start / Pause / Resume / Confirm |
| Back | Back / Exit session |
| Left/Right | Change setting values |

## Versioning

- Version is tracked in `VERSION`
- Every commit bumps version with semver
- Every release commit gets a matching git tag `vX.Y.Z`
