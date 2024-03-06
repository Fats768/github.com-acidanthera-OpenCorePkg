<div align=center>
  <img src="/Docs/Logos/Logo.png" width="200" height="200"/>
  <h1>OpenCore Bootloader</h1><p><i>Development SDK included</i></p>
  <img alt="Build Status" src="https://github.com/acidanthera/OpenCorePkg/actions/workflows/build.yml/badge.svg?branch=master">
  <img alt="Scan Status" src="https://scan.coverity.com/projects/18169/badge.svg?flat=1)](https://scan.coverity.com/projects/18169">
</div>

## :books: Libraries

This repository also contains additional UEFI support common libraries shared by other projects in [Acidanthera](https://github.com/acidanthera). The primary purpose of the library set is to provide supplemental functionality for Apple-specific UEFI drivers. Key features:

- Apple disk image loading support
- Apple keyboard input aggregation
- Apple PE image signature verification
- Apple UEFI secure boot supplemental code
- Audio management with screen reading support
- Basic ACPI and SMBIOS manipulation
- CPU information gathering with timer support
- Cryptographic primitives (SHA-256, RSA, etc.)
- Decompression primitives (zlib, lzss, lzvn, etc.)
- Helper code for ACPI reads and modifications
- Higher level abstractions for files, strings, UEFI variables
- Overflow checking arithmetics
- PE image loading with no UEFI Secure Boot conflict
- Plist configuration format parsing
- PNG image manipulation
- Text output and graphics output implementations
- XNU kernel driver injection and patch engine

Early history of the codebase could be found in [AppleSupportPkg](https://github.com/acidanthera/AppleSupportPkg) and PicoLib library set by The HermitCrabs Lab.

#### :lock: OcGuardLib

This library implements basic safety features recommended for the use within the project. It implements fast
safe integral arithmetics mapping on compiler builtins, type alignment checking, and UBSan runtime,
based on [NetBSD implementation](https://blog.netbsd.org/tnf/entry/introduction_to_µubsan_a_clean).

The use of UBSan runtime requires the use of Clang compiler and `-fsanitize=undefined` argument. Refer to
[Clang documentation](https://releases.llvm.org/7.0.0/tools/clang/docs/UndefinedBehaviorSanitizer.html) for more
details.

#### :star2: Credits

- The HermitCrabs Lab
- All projects providing third-party code (refer to file headers)
- [AppleLife](https://applelife.ru) team and user-contributed resources
- Chameleon and Clover teams for hints and legacy
- [al3xtjames](https://github.com/al3xtjames)
- [Andrey1970AppleLife](https://github.com/Andrey1970AppleLife)
- [mhaeuser (ex Download-Fritz)](https://github.com/mhaeuser)
- [Goldfish64](https://github.com/Goldfish64)
- [MikeBeaton](https://github.com/MikeBeaton)
- [nms42](https://github.com/nms42)
- [PMheart](https://github.com/PMheart)
- [savvamitrofanov](https://github.com/savvamitrofanov)
- [usr-sse2](https://github.com/usr-sse2)
- [vit9696](https://github.com/vit9696)

#### :speech_balloon: Discussion

Please refer to the following [list of OpenCore discussion forums](/Docs/FORUMS.md).
