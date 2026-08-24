# OmegaDR Lean 4 project

Rebuild:

```sh
export PATH="$HOME/.elan/bin:$PATH"
cd lean/OmegaDR && lake build
```

Installed via elan (stable channel). Versions recorded in
`docs/spec/toolchain_report.md`.

## Adding Mathlib later

Do NOT add Mathlib now (heavy build, ~minutes-hours + ~5 GB cache).
When needed (wave 2+, for Chernoff/Hoeffding probability bounds):

1. `cd lean/OmegaDR`
2. Add to `lakefile.lean`:
   ```lean
   require mathlib from git
     "https://github.com/leanprover-community/mathlib4.git" @ "latest"
   ```
3. `lake exe cache get` (fetches prebuilt oleans; needs `elan` and `curl`)
4. `lake build`

Alternatively use `lake new` scaffolding with `mathlib` template or the
`curl https://raw.githubusercontent.com/leanprover-community/mathlib4/master/scripts/upgrade_repository.sh | sh` helper.
Pin a Mathlib release matching the exact `lean-toolchain` of this repo.
