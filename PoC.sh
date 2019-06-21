
wget https://github.com/git-lfs/git-lfs/releases/download/v2.7.2/git-lfs-linux-amd64-v2.7.2.tar.gz
mkdir git-lfs
tar -C git-lfs -zxvf git-lfs-linux-amd64-v2.7.2.tar.gz
sudo bash git-lfs/install.sh
git clone --recursive module_routing_testing git@github.com:Smlight/apollo.git
cd apollo
git checkout module_routing_testing 
bash docker/scripts/dev_start.sh
bash docker/scripts/dev_into.sh
sudo apt update
sudo apt install python3-setuptools
cd protofuzz
sudo python3 setup.py install
cd ..
python3 modules/fuzzing/gen.py
bash apollo.sh build
nohup ./bazel-bin/modules/routing/routing_main

