/*
OpenCASCADE Technology
GNU LESSER GENERAL PUBLIC LICENSE(LGPL) version 2.1
https://www.opencascade.com/content/licensing
*/

#include <stdio.h>
#if 10	//Winows
#include "windows.h"
#include <imagehlp.h>					//MakeSureDirectoryPathExists
#pragma comment(lib, "imagehlp.lib")

#include <shlwapi.h>					//PathIsDirectoryA
#pragma comment(lib, "shlwapi.lib")
#endif

#include <Standard_TypeDef.hxx> // Standard_Boolean
#include <Standard_Macro.hxx> // Handle()
#include <TopAbs_ShapeEnum.hxx> // ShapeType
#include <TopoDS.hxx> // TopoDS::
#include <TopoDS_Shape.hxx> // TopoDS_Shape
#include <TopoDS_Face.hxx> // TopoDS_Face
#include <TopoDS_Edge.hxx> 
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopTools_ListOfShape.hxx>

#include <IGESControl_Controller.hxx>
#include <IGESControl_Reader.hxx> // IGES Reader
#include <IGESData_IGESEntity.hxx>
#include <IGESData_IGESModel.hxx>
#include <IGESCAFControl.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <IGESData_LabelDisplayEntity.hxx>
#include <IGESData_ColorEntity.hxx>
#include <IGESControl_Writer.hxx>

#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx> // STEP Reader
#include <STEPData_STEPModel.hxx>
#include <StepRepr_Representation.hxx>
#include <STEPCAFControl_Reader.hxx>


#include <BRepTools.hxx> // BRepTools::Read()
#include <BRep_Tool.hxx> // BRep_Tool::Triangulation()
#include <BRep_Builder.hxx>
#include <BRepMesh.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopExp_Explorer.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepToIGES_BRSolid.hxx>
#include <XSControl_TransferReader.hxx>
#include <XSControl_WorkSession.hxx>
#include <Transfer_TransientProcess.hxx>
#include <TransferBRep.hxx>

#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Quantity_Color.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDF_ChildIterator.hxx>

#include <Interface_Static.hxx>

#include <Geom_BezierSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Transfer_FinderProcess.hxx>
#include <IGESBasic_Name.hxx>
#include <TransferBRep_ShapeMapper.hxx>
#include <Geom_Surface.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <TColStd_Array2OfReal.hxx>

#include <string>
#include "cylinder.h"

//Because it is troublesome
//all !!
#pragma comment( lib, "TKBin.lib")
#pragma comment( lib, "TKBinL.lib")
#pragma comment( lib, "TKBinTObj.lib")
#pragma comment( lib, "TKBinXCAF.lib")
#pragma comment( lib, "TKBO.lib")
#pragma comment( lib, "TKBool.lib")
#pragma comment( lib, "TKBRep.lib")
#pragma comment( lib, "TKCAF.lib")
#pragma comment( lib, "TKCDF.lib")
#pragma comment( lib, "TKD3DHost.lib")
#pragma comment( lib, "TKDCAF.lib")
#pragma comment( lib, "TKDraw.lib")
#pragma comment( lib, "TKernel.lib")
#pragma comment( lib, "TKFeat.lib")
#pragma comment( lib, "TKFillet.lib")
#pragma comment( lib, "TKG2d.lib")
#pragma comment( lib, "TKG3d.lib")
#pragma comment( lib, "TKGeomAlgo.lib")
#pragma comment( lib, "TKGeomBase.lib")
#pragma comment( lib, "TKHLR.lib")
#pragma comment( lib, "TKIGES.lib")
#pragma comment( lib, "TKIVtk.lib")
#pragma comment( lib, "TKIVtkDraw.lib")
#pragma comment( lib, "TKLCAF.lib")
#pragma comment( lib, "TKMath.lib")
#pragma comment( lib, "TKMesh.lib")
#pragma comment( lib, "TKMeshVS.lib")
#pragma comment( lib, "TKOffset.lib")
#pragma comment( lib, "TKOpenGl.lib")
#pragma comment( lib, "TKPrim.lib")
#pragma comment( lib, "TKQADraw.lib")
#pragma comment( lib, "TKService.lib")
#pragma comment( lib, "TKShHealing.lib")
#pragma comment( lib, "TKStd.lib")
#pragma comment( lib, "TKStdL.lib")
#pragma comment( lib, "TKSTEP.lib")
#pragma comment( lib, "TKSTEP209.lib")
#pragma comment( lib, "TKSTEPAttr.lib")
#pragma comment( lib, "TKSTEPBase.lib")
#pragma comment( lib, "TKSTL.lib")
#pragma comment( lib, "TKTObj.lib")
#pragma comment( lib, "TKTObjDRAW.lib")
#pragma comment( lib, "TKTopAlgo.lib")
#pragma comment( lib, "TKTopTest.lib")
#pragma comment( lib, "TKV3d.lib")
#pragma comment( lib, "TKVCAF.lib")
#pragma comment( lib, "TKViewerTest.lib")
#pragma comment( lib, "TKVRML.lib")
#pragma comment( lib, "TKXCAF.lib")
#pragma comment( lib, "TKXDEDRAW.lib")
#pragma comment( lib, "TKXDEIGES.lib")
#pragma comment( lib, "TKXDESTEP.lib")
#pragma comment( lib, "TKXMesh.lib")
#pragma comment( lib, "TKXml.lib")
#pragma comment( lib, "TKXmlL.lib")
#pragma comment( lib, "TKXmlTObj.lib")
#pragma comment( lib, "TKXmlXCAF.lib")
#pragma comment( lib, "TKXSBase.lib")
#pragma comment( lib, "TKXSDRAW.lib")


Handle(TDocStd_Document) hDoc;
Handle(XCAFApp_Application) hApp;
Handle(XCAFDoc_ShapeTool) aShapeTool;
Handle(XCAFDoc_ColorTool) aColorTool;

char* FileNameExchange(char* filename)
{
	char* p = filename;
	while (*p != '\0')
	{
		int stat = PathGetCharTypeA(*p);
		if (stat == GCT_INVALID || stat & GCT_SEPARATOR || stat & GCT_WILD)
		{
			*p = '-';
		}
		p++;
	}
	return filename;
}

void FileNameLengthModify(char* fname, int maxfullpath_length= _MAX_PATH)
{
	char drive[_MAX_DRIVE];	// ドライブ名
	char dir[_MAX_DIR];		// ディレクトリ名
	char filename[_MAX_FNAME];	// ファイル名
	char ext[_MAX_EXT];		// 拡張子

	//検査したいファイル名を分解してdriveとdirを取得する
	_splitpath(fname, drive, dir, filename, ext);

	//cout << "Drive=" << drive << endl;
	//cout << "Dir  =" << dir << endl;
	//cout << "Fname=" << filename << endl;
	//cout << "Ext  =" << ext << endl;

	//以下で相対パス ..\..\ 等を絶対パスにして本当のパス長さを得る

	//えられたdriveとdirからダミーファイル名を作成して
	char dummy[_MAX_PATH];
	sprintf(dummy, "%s%sdummy_%d", drive, dir, rand());

	//Fullパス名に変換
	char szFullPath[_MAX_PATH] = { '\0' };
	char *szFilePart;
	DWORD dwRet = GetFullPathNameA(fname, _MAX_PATH, szFullPath, &szFilePart);
	if (dwRet != 0)
	{
		//検査したいファイル名を分解してdriveとdirを取得する
		_splitpath(szFullPath, drive, dir, filename, ext);
	}

	int len = strlen(filename) + strlen(drive) + strlen(dir) + strlen(ext) + 10;
	if (len >= maxfullpath_length)
	{

		int n = len - maxfullpath_length;
		printf("n:%d\n", n);
		if (strlen(filename)- n <= 0)
		{
			printf("FileName Length Over!!");
			return;
		}
		char filename2[_MAX_FNAME];	// ファイル名
		memset(filename2, '\0', sizeof(filename2) / sizeof(char));
		strncpy(filename2, filename, strlen(filename) - n);
		strcat(filename2, "-!etc!-");

		sprintf(fname, "%s%s%s%s", drive, dir, filename2, ext);
	}

	dwRet = GetFullPathNameA(fname, _MAX_PATH, szFullPath, &szFilePart);
	if (dwRet != 0)
	{
		strcpy(fname, szFullPath);
	}
}

void AddName(IGESControl_Writer& writer, TopoDS_Shape& shape, char* addname)
{
	Handle(Transfer_FinderProcess) tfp = writer.TransferProcess();
	Handle(IGESData_IGESEntity) igsent;
	Handle(TransferBRep_ShapeMapper) mapper = TransferBRep::ShapeMapper(tfp, shape);
	if (tfp->FindTypedTransient(mapper, STANDARD_TYPE(IGESData_IGESEntity), igsent))
	{
		igsent->SetLabel(new TCollection_HAsciiString(addname));
		//Add name
		Handle(TCollection_HAsciiString) name = new TCollection_HAsciiString("AAA");
		Handle(IGESBasic_Name) nameEntity = new IGESBasic_Name();
		nameEntity->Init(1, name);
		igsent->AddProperty(nameEntity);
	}

}
int main(int argc, char** argv)
{
	printf("This program uses Open CASCADE Technology software library.(OCCT)\n");
	printf("OCCT Copyright (c) 1992-1999 Matra Datavision\n");
	printf("OCCT Copyright (c) 1999-2014 OPEN CASCADE SAS\n");

	if (argc < 2)
	{
		printf("igecnv_write.exe CPFile -o IGESfile\n");
		return -1;
	}

	IGESControl_Writer writer;

	char* inFile = NULL;
	char* output = NULL;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			output = argv[i + 1];
			i++;
			continue;
		}
		else
		{
			inFile = argv[i];
		}
	}
	if (inFile == NULL || output == NULL)
	{
		return -1;
	}

	char buf[256];
	FILE* fp = fopen(inFile, "r");
	if (fp == NULL)
	{
		printf("file open error[%s]\n", inFile);
		return 0;
	}

	int type;
	fgets(buf, 256, fp);
	sscanf(buf, "%d", &type);

	int upatch, vpatch;
	fgets(buf, 256, fp);
	sscanf(buf, "%d %d", &upatch, &vpatch);

	std::vector<TColgp_Array2OfPnt> polesList;

	if (type >= 2)
	{
		TColgp_Array2OfPnt poles(1, 3 * upatch + 1, 1, 3 * vpatch + 1);

		for (int vi = 1; vi <= 3 * vpatch + 1; vi++)
		{
			for (int uj = 1; uj <= 3 * upatch + 1; uj++)
			{
				double x, y, z;
				fgets(buf, 256, fp);
				sscanf(buf, "%lf %lf %lf", &x, &y, &z);
				poles.SetValue(uj, vi, gp_Pnt(x, y, z));
			}
		}
		polesList.push_back(poles);
	}

	if (type == 1)
	{
		bool fend = false;
		int k = 0;
		for (int i = 0; i < upatch*vpatch; i++)
		{
			TColgp_Array2OfPnt poles(0, 3, 0, 3);
			double cp[4][4][3];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					fgets(buf, 256, fp);
					sscanf(buf, "%lf %lf %lf", &cp[i][j][0], &cp[i][j][1], &cp[i][j][2]);
					poles.SetValue(j, i, gp_Pnt(cp[i][j][0], cp[i][j][1], cp[i][j][2]));
				}
				if (fend) break;
			}
			polesList.push_back(poles);
		}
	}

	if (type == 0)
	{
		bool fend = false;
		do
		{
			TColgp_Array2OfPnt poles(0, 3, 0, 3);
			double cp[4][4][3];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (!fgets(buf, 256, fp))
					{
						fend = true;
						break;
					}
					sscanf(buf, "%lf %lf %lf", &cp[i][j][0], &cp[i][j][1], &cp[i][j][2]);
					poles.SetValue(j, i, gp_Pnt(cp[i][j][0], cp[i][j][1], cp[i][j][2]));
				}
				if (fend) break;
			}
			if (!fend)
			{
				polesList.push_back(poles);
			}
		} while (!fend);
	}

	BRep_Builder aBCompound;
	TopoDS_Compound compound;
	aBCompound.MakeCompound(compound);

	BRep_Builder aBSolid;
	TopoDS_Solid solid;
	aBSolid.MakeSolid(solid);

	BRep_Builder aBShell;
	TopoDS_Shell shellF;
	aBShell.MakeShell(shellF);


	if (type == 0 || type == 1)
	{
		for (int i = 0; i < polesList.size(); i++)
		{
			Handle(Geom_BezierSurface) s = new Geom_BezierSurface(polesList[i]);
			TopoDS_Face f = BRepBuilderAPI_MakeFace(s, 1.0e-7);
			aBShell.Add(shellF, f);
		}
	}

#if 10
	if (type >= 2 )
	{
		for (int i = 0; i < polesList.size(); i++)
		{
			const int UDegree = 3;
			const int VDegree = 3;

			int p = UDegree;
			int n = 3 * upatch;	//n+1= NP ->number of control points
			int m = n + p + 1;	//m+1= Nk ->number of the knots
								// Nk = NP + p + 1 

			printf("m(%d)=n(%d)+p(%d)+1=%d\n", m, n, p, n + p + 1);

			//BezierPatch Form B-Spline
			if (type == 2)
			{
				TColStd_Array1OfReal UKnots(1, upatch + 1);
				TColStd_Array1OfInteger UMults(1, upatch + 1);

				TColStd_Array1OfReal VKnots(1, vpatch + 1);
				TColStd_Array1OfInteger VMults(1, vpatch + 1);

				UKnots.SetValue(1, 0);
				UMults.SetValue(1, 4);
				for (int j = 2; j <= UKnots.Length() - 1; j++)
				{
					UKnots.SetValue(j, j);
					UMults.SetValue(j, 3);
				}
				UKnots.SetValue(UKnots.Length(), UKnots.Length());
				UMults.SetValue(UKnots.Length(), 4);

				VKnots.SetValue(1, 0);
				VMults.SetValue(1, 4);
				for (int j = 2; j <= VKnots.Length() - 1; j++)
				{
					VKnots.SetValue(j, j);
					VMults.SetValue(j, 3);
				}
				VKnots.SetValue(VKnots.Length(), VKnots.Length());
				VMults.SetValue(VKnots.Length(), 4);

				Handle(Geom_BSplineSurface) bsp = new Geom_BSplineSurface(polesList[i], UKnots, VKnots, UMults, VMults, UDegree, VDegree, false, false);
				TopoDS_Face f = BRepBuilderAPI_MakeFace(bsp, 1.0e-7);
				aBShell.Add(shellF, f);
			}

			if (type == 3)
			{
				//B-Spline
				TColStd_Array1OfReal UKnots(1, 3 * upatch + 1 + UDegree + 1 - 6);
				TColStd_Array1OfInteger UMults(1, 3 * upatch + 1 + UDegree + 1 - 6);

				TColStd_Array1OfReal VKnots(1, 3 * vpatch + 1 + VDegree + 1 - 6);
				TColStd_Array1OfInteger VMults(1, 3 * vpatch + 1 + VDegree + 1 - 6);

				UKnots.SetValue(1, 0);
				UMults.SetValue(1, 4);
				for (int j = 2; j <= UKnots.Length() - 1; j++)
				{
					UKnots.SetValue(j, j);
					UMults.SetValue(j, 1);
				}
				UKnots.SetValue(UKnots.Length(), UKnots.Length());
				UMults.SetValue(UKnots.Length(), 4);

				VKnots.SetValue(1, 0);
				VMults.SetValue(1, 4);
				for (int j = 2; j <= VKnots.Length() - 1; j++)
				{
					VKnots.SetValue(j, j);
					VMults.SetValue(j, 1);
				}
				VKnots.SetValue(VKnots.Length(), VKnots.Length());
				VMults.SetValue(VKnots.Length(), 4);

				Handle(Geom_BSplineSurface) bsp = new Geom_BSplineSurface(polesList[i], UKnots, VKnots, UMults, VMults, UDegree, VDegree, false, false);
				TopoDS_Face f = BRepBuilderAPI_MakeFace(bsp, 1.0e-7);
				aBShell.Add(shellF, f);
			}

			//Nurbs
			if (type == 4)
			{
				//B-Spline
				TColStd_Array1OfReal UKnots(1, 3 * upatch + 1 + UDegree + 1 - 6);
				TColStd_Array1OfInteger UMults(1, 3 * upatch + 1 + UDegree + 1 - 6);

				TColStd_Array1OfReal VKnots(1, 3 * vpatch + 1 + VDegree + 1 - 6);
				TColStd_Array1OfInteger VMults(1, 3 * vpatch + 1 + VDegree + 1 - 6);

				TColStd_Array2OfReal Weights(1, 3 * upatch + 1, 1, 3 * vpatch + 1);

				int ki = 1;
				int kj = 1;
				for (int i = 1; i <= 3 * vpatch + 1; i++, ki++)
				{
					for (int j = 1; j <= 3 * upatch + 1; j++, kj++)
					{
						if (ki == 4) ki = 1;
						if (kj == 4) kj = 1;
						if ((ki == 2 || ki == 3) && (kj == 2 || kj == 3))
						{
							Weights.SetValue(j, i, 0.9);
						}
						else
						{
							Weights.SetValue(j, i, 0.5);
						}
					}
				}
				UKnots.SetValue(1, 0);
				UMults.SetValue(1, 4);
				for (int j = 2; j <= UKnots.Length() - 1; j++)
				{
					UKnots.SetValue(j, j);
					UMults.SetValue(j, 1);
				}
				UKnots.SetValue(UKnots.Length(), UKnots.Length());
				UMults.SetValue(UKnots.Length(), 4);

				VKnots.SetValue(1, 0);
				VMults.SetValue(1, 4);
				for (int j = 2; j <= VKnots.Length() - 1; j++)
				{
					VKnots.SetValue(j, j);
					VMults.SetValue(j, 1);
				}
				VKnots.SetValue(VKnots.Length(), VKnots.Length());
				VMults.SetValue(VKnots.Length(), 4);

				Handle(Geom_BSplineSurface) bsp = new Geom_BSplineSurface(polesList[i], Weights, UKnots, VKnots, UMults, VMults, UDegree, VDegree, false, false);
				TopoDS_Face f = BRepBuilderAPI_MakeFace(bsp, 1.0e-7);
				aBShell.Add(shellF, f);
			}
		}
	}
#else	
	if (type == 2)
	{
		for (int i = 0; i < polesList.size(); i++)
		{
			GeomAPI_PointsToBSplineSurface gps;
			gps.Interpolate(polesList[i]);
			Handle(Geom_BSplineSurface) bs = gps.Surface();
			TopoDS_Face f = BRepBuilderAPI_MakeFace(bs, 1.0e-7);
			aBShell.Add(shellF, f);
		}
	}
#endif


	aBSolid.Add(solid, shellF);
	aBCompound.Add(compound, solid);

	writer.AddShape(compound);
	AddName(writer, solid, "solid");
	AddName(writer, shellF, "shell");

	{
		char drive[_MAX_DRIVE];		// drive name
		char dir[_MAX_DIR];			// diretory name
		char filename[_MAX_FNAME];	// file name
		char ext[_MAX_EXT];			// extension

		_splitpath(output, drive, dir, filename, ext);

		AddName(writer, compound, filename);
	}

	writer.Write(output);

	return 0;
}