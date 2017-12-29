# pdb2xyz
Converter from PDB format to XYZ. Can write xyz-file with only CA (or any others) atoms.
Keep track of missing atoms. Can take the longest part without missing atoms.
Exclude repeated atoms.
Can choose one or several chains or models from the pdb-file.

Read Manual for more information:
```bash
$ man ./pdb2xyz.1
```
## Quick Start
Write in terminal:
```bash
$ git clone https://github.com/Anny-Moon/pdb2xyz
$ cd pdb2xyz/
$ sh install.sh
$ ./pdb2xyz.out 5dn7 -f CA --part
```