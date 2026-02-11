# Keyboard + Vim Cheat Sheet

Sofle v2 Choc | Colemak Magic Sturdy | Neovim + Tmux

---

## BASE Layer (Colemak Magic Sturdy)

```
 `    1    2    3    4    5              6    7    8    9    0   Bspc
Tab   Q    W    F    P    B              J    L    U    Y    '   Del
Esc  G/A  A/R  S/S  C/T   G              M   C/N  S/E  A/I  G/O  Ent
Caps  Z    X    C    D    V   Mute Play   K    H    ,    .    /   RSft
          LAlt LGui TMUX RAISE OCtl AREP LOWER MAINT GAMER BASE+
               |    Tab  Bspc           Spc   Rep
```

Home row mods: **G**ui **A**lt **S**hift **C**trl (GASC order, both hands)

## Thumb Keys

| Key | Tap | Hold |
|-----|-----|------|
| Left inner | Tab | TMUX layer |
| Left middle | Bspc | RAISE layer |
| Left outer | - | One-shot Ctrl |
| Right outer | - | Alt Repeat (Magic) |
| Right middle | Spc | LOWER layer |
| Right inner | Repeat | MAINTENANCE layer |

---

## LOWER Layer (hold Spc) - Symbols + Navigation

```
 `    1    2    3    4    5             C-Lf C-Dn C-Up C-Rt  0   Bspc
Tab  ::  SelWd  ]    )    }             W-Lf Home End  W-Rt PgUp Del
Esc   =    |    [    (    {              <-   Dn   Up   ->  PgDn Ent
LSft  \    /    +    ;    -              W-Bs  :    _   W-Dl  /  RSft
```

| Key | What it does |
|-----|-------------|
| `::` | Types `::` (scope) |
| `SelWd` | Select word under cursor (tap again to expand) |
| `C-Lf/Rt` | Ctrl+Arrow (macOS workspace switching) |
| `C-Up/Dn` | Ctrl+Up/Down (macOS Mission Control) |
| `W-Lf/Rt` | Word jump left/right (Alt on mac, Ctrl on win/linux) |
| `W-Bs/Dl` | Delete word back/forward (OS-aware) |
| `<- Dn Up ->` | Arrow keys |
| Encoder L | Word left / Word right |
| Encoder R | PgUp / PgDn |

---

## RAISE Layer (hold Bspc) - Nvim/Editor

```
 `   Hrp1 Hrp2 Hrp3 Hrp4 Hrp5                                 Bspc
Tab  :q         Splt Find Grep          S-H  C-D  C-U  S-L        Del
Esc       Fmt  CdAc Renm  CPR          PnLf PnDn PnUp PnRt       Ent
LSft GSta GBlm       diw  :w           HpMn HpPv HpNx HpAd      RSft
```

| Key | Nvim action |
|-----|-------------|
| `Hrp1-5` | Harpoon: jump to file 1-5 (`<leader>h1-5`) |
| `:q` | Quit (`:q<CR>`) |
| `Splt` | Split helper (`Cmd+K`) |
| `Find` | Find files (`<leader>ff`) |
| `Grep` | Live grep (`<leader>ft`) |
| `Fmt` | LSP format (`<leader>lf`) |
| `CdAc` | Code action (`<leader>la`) |
| `Renm` | Rename symbol (`<leader>ln`) |
| `CPR` | Search & replace (`:%s///g` with cursor placed) |
| `PnLf/Dn/Up/Rt` | Pane navigation (`C-w h/j/k/l`) |
| `S-H / S-L` | Buffer top / bottom of screen |
| `C-D / C-U` | Half-page down / up |
| `GSta` | Git stage hunk (`<leader>gs`) |
| `GBlm` | Git blame (`<leader>gb`) |
| `diw` | Delete inner word |
| `:w` | Save file |
| `HpMn` | Harpoon menu (`<leader>hh`) |
| `HpPv/Nx` | Harpoon prev/next (`.` and `,`) |
| `HpAd` | Harpoon add file (`<leader>ha`) |
| Encoder L | S-H / S-L (buffer scroll) |
| Encoder R | C-U / C-D (half-page scroll) |

---

## TMUX Layer (hold Tab) - Session Management

```
 `                                                              Bspc
Tab KillS KillP Prev Next ChSh         Ssnzr LastW Inst       Zoom Del
Esc Relod Restr Save  New Renam         SwLf  SwDn  SwUp SwRt CpyMd Ent
LSft            Sesns Dtch SpltV              SpltH               RSft
```

| Key | Tmux action (prefix = C-b) |
|-----|---------------------------|
| `KillS` | Kill session (`C-b q`) |
| `KillP` | Kill pane (`C-b w`) |
| `Prev/Next` | Previous/next window (`C-b p/n`) |
| `ChSh` | Install plugins (`C-b i`) |
| `Ssnzr` | Sessionizer (`C-b f`) |
| `LastW` | Last window (`C-b l`) |
| `Inst` | Update plugins (`C-b Shift+U`) |
| `Zoom` | Toggle zoom (`C-b z`) |
| `Relod` | Reload config (`C-b r`) |
| `Restr` | Restore session (`C-b C-r`) |
| `Save` | Save session (`C-b C-s`) |
| `New` | New window (`C-b c`) |
| `Renam` | Rename window (`C-b ,`) |
| `SwLf/Dn/Up/Rt` | Switch pane (`C-b arrow`) |
| `CpyMd` | Copy mode (`C-b [`) |
| `Sesns` | Sessions list (`C-b s`) |
| `Dtch` | Detach (`C-b d`) |
| `SpltV/H` | Split vertical/horizontal (`C-b v/h`) |
| Encoder L | Prev/Next window |

---

## MAINTENANCE Layer (hold Rep) - System/Mouse/RGB

```
 `   Boot                                                       Bspc
Tab  Exit RGB- RGB+ RGBm LLck         MwUp MBtn1 MUp MBtn2 Turbo Del
Esc       Hue- Hue+                    MwDn MLft  MDn MRgt MSlow Ent
LSft      Sat- Sat+                          MBtn3               RSft
```

| Key | What |
|-----|------|
| `Boot` | Bootloader mode (flash firmware) |
| `Exit` | Exit maintenance layer |
| `LLck` | Layer Lock (stays on layer without holding) |
| `RGB-/+` | RGB brightness down/up |
| `RGBm` | Next RGB mode |
| `Hue-/+` | RGB hue |
| `Sat-/+` | RGB saturation |
| `MUp/Dn/Lf/Rt` | Orbital mouse movement |
| `MBtn1/2/3` | Mouse buttons |
| `MwUp/Dn` | Mouse wheel |
| `MSlow` | Slow mouse mode |
| `Turbo` | Turbo click |

---

## Nvim Leader Keybindings (Space = leader)

### Find (`<leader>f`)
| Key | Action |
|-----|--------|
| `ff` | Find files (Telescope) |
| `ft` | Search text (live grep) |
| `fb` | Find buffers |
| `fh` | Find help tags |

### LSP (`<leader>l`)
| Key | Action |
|-----|--------|
| `lf` | Format code (conform) |
| `ld` | Go to definition |
| `lr` | References |
| `li` | Go to implementation |
| `lt` | Type definition |
| `lh` | Hover documentation |
| `la` | Code actions |
| `ln` | Rename symbol |
| `lD` | Line diagnostics |

### Git (`<leader>g`)
| Key | Action |
|-----|--------|
| `gs` | Stage hunk |
| `gr` | Reset hunk |
| `gS` | Stage buffer |
| `gp` | Preview hunk |
| `gb` | Blame line |
| `gd` | Diff this |

### Debug (`<leader>d`)
| Key | Action |
|-----|--------|
| `db` | Toggle breakpoint |
| `dB` | Conditional breakpoint |
| `dc` | Continue/start |
| `di` | Step into |
| `do` | Step over |
| `dO` | Step out |
| `dr` | Restart |
| `dq` | Terminate |
| `du` | Toggle DAP UI |
| `de` | Eval expression |

### Test (`<leader>T`)
| Key | Action |
|-----|--------|
| `Tn` | Run nearest test |
| `Tf` | Run file tests |
| `Ts` | Toggle summary |
| `To` | Show output |
| `Td` | Debug nearest test |
| `Tq` | Stop test |
| `]T` / `[T` | Next/prev failed test |

### Harpoon (`<leader>h`)
| Key | Action |
|-----|--------|
| `ha` | Add file |
| `hh` | Toggle menu |
| `h1-5` | Jump to file 1-5 |
| `.` | Next file |
| `,` | Previous file |

### Code/AI (`<leader>c`)
| Key | Action |
|-----|--------|
| `cp` | Copilot panel |
| `cc` | Copilot Chat |
| `ce/cd` | Enable/disable Copilot |

### Tabs (`<leader>t`)
| Key | Action |
|-----|--------|
| `tn` | New tab |
| `tc` | Close tab |
| `to` | Only this tab |

---

## Vim Essentials (Normal Mode)

### Movement
| Key | Action |
|-----|--------|
| `h j k l` | Left, down, up, right |
| `w / b` | Next/prev word |
| `e` | End of word |
| `0 / $` | Start/end of line |
| `gg / G` | Top/bottom of file |
| `C-d / C-u` | Half-page down/up (smooth scroll) |
| `{ / }` | Prev/next paragraph |
| `%` | Matching bracket |
| `gd` | Go to definition |
| `gr` | Go to references |
| `K` | Hover docs |
| `[d / ]d` | Prev/next diagnostic |

### Editing
| Key | Action |
|-----|--------|
| `i / a` | Insert before/after cursor |
| `I / A` | Insert at start/end of line |
| `o / O` | New line below/above |
| `x` | Delete char |
| `dd` | Delete line |
| `yy` | Yank (copy) line |
| `p / P` | Paste after/before |
| `u / C-r` | Undo / redo |
| `ciw` | Change inner word |
| `ci"` | Change inside quotes |
| `di(` | Delete inside parens |
| `.` | Repeat last change |
| `>>` / `<<` | Indent / unindent |

### Visual Mode
| Key | Action |
|-----|--------|
| `v` | Visual (char) |
| `V` | Visual (line) |
| `C-v` | Visual (block) |
| `J / K` | Move selection down/up |

---

## Special Features

| Feature | How to activate |
|---------|----------------|
| **Caps Word** | Press `C` + `,` together (combo) |
| **Select Word** | `SelWd` on LOWER (tap to expand selection) |
| **Layer Lock** | `LLck` on MAINTENANCE (locks current layer) |
| **Magic Key** | Alt Repeat (right thumb) - context-dependent |
| **SOCD Cleaner** | Auto-enabled on GAMER layer |
| **Repeat Key** | Right inner thumb - repeats last key |
| **OS Detection** | Word nav auto-switches Alt/Ctrl per OS |

### Magic Key Combos
```
A * -> AO     L * -> LK      S * -> SK
C * -> CY     M * -> MENT    T * -> TMENT
D * -> DY     O * -> OA      U * -> UE
E * -> EU     P * -> PY      Y * -> YP
G * -> GY     Q * -> QUEN    spc * -> THE
I * -> ION    R * -> RL
= * -> ===    ! * -> !==
" * -> """_"""   ` * -> ```_```
# * -> #include    . * -> ../
```

---

## Quick Reference

```
LOWER (Spc)  = Symbols left, Nav right
RAISE (Bspc) = Nvim macros left, Panes/Harpoon right
TMUX  (Tab)  = Full tmux management
MAINT (Rep)  = RGB, Mouse, System
```

Encoders:
```
BASE:  Vol-/Vol+  |  Prev/Next track
LOWER: Word L/R   |  PgUp/PgDn
RAISE: S-H/S-L    |  C-U/C-D
MAINT: RGB Br-/+  |  RGB mode prev/next
TMUX:  Prev/Next  |  Vol-/Vol+
```
