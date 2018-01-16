# IGES-STEP-to-OBJ
based on OpenCASCADE

igescnv.exe IGESorSTEP_File -o objbasename -edge [0,1]
 -one [0,1] -timeout min
 
 ex)
 igescnv.exe model.igs -o model
 
<img src="https://github.com/Sanaxen/IGES-STEP-to-OBJ/blob/master/image/image2.png"/> 
 This OBJViewer is Meshlab.

ex)
igescnv_write.exe CP.dat -o igesFile.igs

CP.dat　is the control point data file.
https://github.com/Sanaxen/OBJ-to-IGES
