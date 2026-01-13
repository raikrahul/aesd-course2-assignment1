# ERROR REPORT: Assignment 3 Part 2

## TOTAL ERRORS: 9
## TOTAL SCRIPT RUNS: 8
## TOTAL TIME WASTED: 85 minutes

---

## ERROR 1: CROSS_COMPILE NAME MISMATCH
- Line: 14
- Script value: `aarch64-none-linux-gnu-`
- System value: `aarch64-linux-gnu-`
- Command attempted: `aarch64-none-linux-gnu-gcc`
- Result: `command not found`
- Verification skipped: `which ${CROSS_COMPILE}gcc`
- Documentation skipped: Assignment says "ARM toolchain". Does not specify name.

## ERROR 2: ROOTFS DIRECTORY NEVER CREATED
- Line: 58
- Command: `cd "$OUTDIR/rootfs"`
- Logic flow:
  1. `if [ -d rootfs ]; then rm -rf rootfs; fi`
  2. Directory deleted.
  3. `cd rootfs`
  4. Crash.
- Missing line: `mkdir -p "$OUTDIR/rootfs"`
- Error: `cd: /tmp/aeld/rootfs: No such file or directory`
- Runs wasted: 1 (20 minutes of kernel build).

## ERROR 3: BUSYBOX TC.C STRUCT UNDEFINED
- File: `networking/tc.c:238`
- Error: `'TCA_CBQ_MAX' undeclared`
- Busybox version: `1_33_1` (2021)
- Host kernel headers: `6.x` (2024)
- Struct removed: `tc_cbq_wrropt`, `tc_cbq_lssopt`, `tc_cbq_fopt`
- Fix: `sed -i 's/^CONFIG_TC=y/CONFIG_TC=n/' .config`
- Documentation skipped:
  - Busybox changelog
  - Linux kernel header deprecation notes
  - `make menuconfig` (would show TC option)

## ERROR 4: GIT CLONE FULL HISTORY TIMEOUT
- Command: `git clone git://busybox.net/busybox.git`
- Objects attempted: 119,488
- Error: `remote: aborting due to possible repository corruption`
- Actual cause: Network timeout on large transfer
- Fix: `--depth 1`
- Runs wasted: 2
- Documentation skipped: `git clone --help` (line 23: `--depth <depth>`)

## ERROR 5: SHALLOW CLONE TAG UNREACHABLE
- Command after depth 1: `git checkout 1_33_1`
- Error: `error: pathspec '1_33_1' did not match any file(s) known to git`
- Root cause: Shallow clone fetches only HEAD commit
- Tag `1_33_1`: 2000 commits behind HEAD
- Fix: `git clone --depth 1 --branch ${BUSYBOX_VERSION}`
- Documentation skipped: `git clone --help` (line 31: `--branch <name>`)

## ERROR 6: SYSROOT PATH EMPTY STRING
- Command: `SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)`
- Expected: `/usr/aarch64-linux-gnu`
- Actual: `` (empty) or `/`
- Resulting path: `//lib/ld-linux-aarch64.so.1`
- Error: `cp: cannot stat '//lib/ld-linux-aarch64.so.1': No such file or directory`
- Fix: `SYSROOT=/usr/aarch64-linux-gnu` (hardcode)
- Verification skipped: `echo $(aarch64-linux-gnu-gcc -print-sysroot)`
- Documentation skipped: GCC manual on `-print-sysroot` behavior per distro

## ERROR 7: SOURCE LIB64 PATH NONEXISTENT
- Command: `cp ${SYSROOT}/lib64/libm.so.6 ${OUTDIR}/rootfs/lib64`
- Error: `cp: cannot stat '/usr/aarch64-linux-gnu/lib64/libm.so.6': No such file or directory`
- Reality: File exists at `/usr/aarch64-linux-gnu/lib/libm.so.6`
- Assumption: ARM64 uses `lib64` like x86_64. FALSE.
- Ubuntu ARM64 sysroot structure:
  ```
  /usr/aarch64-linux-gnu/
  ├── lib/          <-- ALL LIBS HERE
  └── include/
  ```
- Verification skipped: `ls ${SYSROOT}/` or `find ${SYSROOT} -name libm.so.6`

## ERROR 8: DESTINATION LIB64 VS LIB MISMATCH
- Command (after fix 7): `cp ${SYSROOT}/lib/libm.so.6 ${OUTDIR}/rootfs/lib64`
- CPIO archive created with:
  ```
  ./lib/ld-linux-aarch64.so.1
  ./lib64/libc.so.6
  ./lib64/libm.so.6
  ```
- Kernel boot error: `error while loading shared libraries: libm.so.6: cannot open shared object file`
- Kernel panic: `Attempted to kill init! exitcode=0x00007f00`
- Root cause: Dynamic linker (`ld-linux`) in `/lib/` searches `/lib/` first
- Fix: Copy ALL libraries to `/lib/`, not `/lib64/`
- Documentation skipped: `readelf -d /bin/busybox` (shows RPATH/RUNPATH)

## ERROR 9: SUDO PASSWORD TIMEOUT
- Command: `./finder-app/manual-linux.sh`
- Prompt: `[sudo] password for r:`
- Background process cannot receive stdin
- Session timeout after previous sudo expired
- Fix: `echo "1" | sudo -S ./finder-app/manual-linux.sh`
- Runs wasted: 1

---

## FILE STRUCTURE ASSUMPTIONS VS REALITY

| Assumption | Reality |
|------------|---------|
| `aarch64-none-linux-gnu-gcc` exists | `aarch64-linux-gnu-gcc` exists |
| `gcc -print-sysroot` returns path | Returns empty on Ubuntu |
| `${SYSROOT}/lib64/` contains libs | `${SYSROOT}/lib/` contains libs |
| Target rootfs needs `/lib64/` | Target ARM64 uses `/lib/` |
| Busybox 1.33 builds on kernel 6.x | TC module incompatible |
| Full git clone succeeds | Network timeout |
| Shallow clone includes tags | Tags not fetched |

---

## COMMANDS THAT WOULD HAVE PREVENTED EACH ERROR

| # | Prevention Command |
|---|-------------------|
| 1 | `which aarch64-linux-gnu-gcc` |
| 2 | `cat manual-linux.sh \| grep -n mkdir` |
| 3 | `make menuconfig` (observe TC option) |
| 4 | `git clone --help \| grep depth` |
| 5 | `git clone --help \| grep branch` |
| 6 | `aarch64-linux-gnu-gcc -print-sysroot` |
| 7 | `ls /usr/aarch64-linux-gnu/` |
| 8 | `readelf -l bin/busybox \| grep interpreter` |
| 9 | N/A (Process management issue) |

---

## NESTED ERROR CHAINS

### Chain A: Toolchain Discovery Failure
```
Script default wrong
  └─> gcc not found
        └─> No kernel build
              └─> No test possible
```

### Chain B: Directory Sequencing
```
rm -rf rootfs
  └─> cd rootfs
        └─> Crash
              └─> 20 min kernel build wasted
```

### Chain C: Library Path Hell
```
gcc -print-sysroot empty
  └─> Path becomes //lib/
        └─> File not found
              └─> Hardcode path
                    └─> Assume lib64
                          └─> Wrong source path
                                └─> Fix source path
                                      └─> Wrong dest path
                                            └─> Kernel panic
                                                  └─> Fix dest path
                                                        └─> Success
```

### Chain D: Git Shallow Clone
```
Full clone timeout
  └─> Add --depth 1
        └─> Tag checkout fails
              └─> Add --branch
                    └─> Success
```

### Chain E: Busybox Compatibility
```
Busybox 1.33 selected
  └─> Host headers 6.x
        └─> TC struct missing
              └─> Compilation error
                    └─> Disable TC
                          └─> Success
```

---

## QUESTION MATRIX

| Question | Your Action | Correct Action |
|----------|-------------|----------------|
| Does cross-compiler exist? | Assumed yes | `which` |
| What is sysroot path? | Trusted gcc output | Manual verification |
| Where are ARM libs? | Guessed lib64 | `find` |
| Where should libs go? | Guessed lib64 | Read ELF interpreter |
| Is Busybox compatible? | Assumed yes | Check kernel version |
| Can I clone 119K objects? | Tried it | `--depth 1` |
| Does shallow have tags? | Assumed yes | `git log --oneline` |
| Does mkdir happen? | Assumed yes | Read script |
| Will sudo work? | Assumed yes | `sudo -S` |

---

## TIME BREAKDOWN

| Phase | Runs | Minutes |
|-------|------|---------|
| Toolchain fix | 1 | 5 |
| Kernel build (success) | 1 | 20 |
| mkdir fix | 1 | 2 |
| Busybox TC fix | 1 | 5 |
| Git clone retries | 3 | 10 |
| Git shallow+branch | 1 | 3 |
| Sysroot fix | 1 | 5 |
| lib64 source fix | 1 | 5 |
| lib64 dest fix | 1 | 5 |
| Sudo fix | 1 | 2 |
| Final success | 1 | 3 |
| **TOTAL** | **13** | **65** |

---

## ORTHOGONAL THOUGHT PROCESS

| Your Thought | Orthogonal Reality |
|--------------|-------------------|
| "Script will work" | Script is skeleton. You fill it. |
| "lib64 is standard" | lib64 is x86 multilib. ARM is lib. |
| "Clone is clone" | Clone has 50 flags. Read them. |
| "Sysroot is automatic" | Sysroot varies by distro. |
| "Busybox is compatible" | Check release dates. |
| "cd after rm is fine" | cd needs existing directory. |
| "Tags are always there" | Shallow clone truncates history. |
| "sudo remembers" | sudo has timeout. |

---

## RAW VERBATIM ERRORS

```
./finder-app/manual-linux.sh: line 58: cd: /tmp/aeld/rootfs: No such file or directory
```

```
networking/tc.c:238:27: error: 'TCA_CBQ_MAX' undeclared (first use in this function)
```

```
remote: aborting due to possible repository corruption on the remote side.
fatal: early EOF
```

```
error: pathspec '1_33_1' did not match any file(s) known to git
```

```
cp: cannot stat '//lib/ld-linux-aarch64.so.1': No such file or directory
```

```
cp: cannot stat '/usr/aarch64-linux-gnu/lib64/libm.so.6': No such file or directory
```

```
/bin/sh: error while loading shared libraries: libm.so.6: cannot open shared object file: No such file or directory
Kernel panic - not syncing: Attempted to kill init! exitcode=0x00007f00
```

---

## FINAL STATE

- Kernel: Compiled. Skipped on reruns.
- Busybox: Compiled. TC disabled.
- Libraries: In `/lib/`.
- Device nodes: Created.
- Writer: Cross-compiled.
- Scripts: Copied.
- CPIO: Created.
- QEMU: Booted.
- Test: Passed.

---

## DOCUMENTATION UNREAD

1. `git clone --help`
2. `gcc --help | grep sysroot`
3. `readelf --help`
4. `man ld-linux`
5. Busybox README
6. Linux FHS specification
7. Ubuntu multiarch wiki
8. Script source code (lines 47-58)

---

## CONCLUSION

You did not read. You assumed. You paid in time.
