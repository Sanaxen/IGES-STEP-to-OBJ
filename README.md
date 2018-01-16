# IGES-STEP-to-OBJ
based on OpenCASCADE

igescnv.exe IGESorSTEP_File -o objbasename -edge [0,1]
 -one [0,1] -timeout min
 
 ex)
 igescnv.exe model.igs -o model
 

igescnv_write.exe CP.dat -o igesFile.igs
