# [](#header-1)Automated Selection of Hotspots (ASH)

Enhanced automated segmentation and adaptive step finding for Ki67 hotspot detection in adrenal cortical cancer

## [](#header-2)ASH VM

- Download the VM [here](https://bioinf-galaxian.erasmusmc.nl/owncloud/index.php/s/2GXV8yDJdyIm6p4/download).
- Open it with [VMware Workstation Player](https://my.vmware.com/en/web/vmware/free#desktop_end_user_computing/vmware_workstation_player/12_0) or [VirtualBox](https://www.virtualbox.org/).
- Copy your data into the VM and upload it to Galaxy.
- Run the Image Processing tool on the uploaded data.

## [](#header-2)Manual Setup

Dependencies:
- Java
- [ImageJ](http://rsb.info.nih.gov/ij/download/linux/ij147-linux64.zip)
  - [ImmunoRation](http://153.1.200.58/sites/default/files/software/immunoratio-plugin/immunoratio_1.0c.jar)
  - [LOCI](http://cvs.openmicroscopy.org.uk/snapshots/bioformats/4.4.8/loci_tools.jar)
- [OpenCV](http://opencv.org/)
- [ndpisplit](https://www.imnc.in2p3.fr/pagesperso/deroulers/software/ndpitools/)

Pull the code and compile it
```bash
git https://github.com/ErasmusMC-Bioinformatics/ASH-Ki67.git
c++ -Wall Split.cpp -I/usr/local/include/opencv/ -lopencv_core -lopencv_highgui -o split.out
c++ -Wall Overlap.cpp -I/usr/local/include/opencv/ -lopencv_core -lopencv_highgui -o overlap.out
c++ -Wall Annotate.cpp -I/usr/local/include/opencv/ -lopencv_core -lopencv_highgui -o annotate.out
```
