#!/usr/bin/env bash
set -e
wget http://www.lrde.epita.fr/dload/spot/spot-2.12.tar.gz
tar -xzvf spot-2.12.tar.gz
cd spot-2.12; ./configure; cd ..
cd spot-2.12; make -j4; cd ..
git clone https://github.com/progirep/monitor_compiler_for_caraval_monitor
echo "=========================================="
echo "Success in building and downloading the needed libraries!"
echo "=========================================="
