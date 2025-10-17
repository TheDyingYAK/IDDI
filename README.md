# IDDI
this is my repo to automatically hash any PE file that is Created in /root || /home on a linux sustem

## Description

An in-depth paragraph about your project and overview of use.

## Getting Started

### Dependencies

```bash
sudo apt update
sudo apt install libssl-dev

```

### Installing

* How/where to download your program
* Any modifications needed to be made to files/folders

### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

File directory break down 
```text
pe-monitor/
├── src/
│   └── pe_monitor.c          # Main C source code
├── scripts/
│   ├── install.sh            # Main installation script
│   ├── uninstall.sh          # Uninstallation script
│   └── build.sh              # Build script for development
├── systemd/
│   └── pe-monitor.service    # Systemd service unit file
├── packaging/
│   ├── debian/               # Debian package structure (for .deb)
│   │   ├── control
│   │   ├── rules
│   │   └── postinst
│   └── rpm/                  # RPM package structure (optional)
├── docs/
│   ├── README.md            # Project documentation
│   ├── INSTALL.md           # Installation instructions
│   └── USAGE.md             # Usage and troubleshooting
├── tests/
│   ├── test_pe_files/       # Sample PE files for testing
│   │   ├── test1.exe
│   │   └── test2.dll
│   ├── test_non_pe/         # Non-PE files for negative testing
│   │   └── test.txt
│   └── test_monitor.py      # Python script to test monitoring
├── config/
│   ├── pe-monitor.conf      # Configuration file (optional)
│   └── logrotate.d/
│       └── pe-monitor       # Log rotation config
├── man/
│   └── man1/
│       └── pe_monitor.1     # Man page for the binary
└── LICENSE                  # License file (MIT, GPL, etc.)
```

## Authors

Contributors names and contact info

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)
