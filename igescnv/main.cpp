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
#include <process.h>    /* _beginthread, _endthread */  

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
#include <string>

#include "cylinder.h"
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

class Color
{
public:
	float color[3];

	Color()
	{
		color[0] = -1.0;
		color[1] = -1.0;
		color[2] = -1.0;
	}
	Color(float c[3])
	{
		color[0] = c[0];
		color[1] = c[1];
		color[2] = c[2];
	}
	Color(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	Color(Quantity_Color& c)
	{
		color[0] = c.Red();
		color[1] = c.Green();
		color[2] = c.Blue();
	}
};

FILE*	obj_fp   = NULL;
int		vtx_num  = 0;
int		face_num = 0;
int		tri_num  = 0;
int		vindex   = 0;
Color	parentColor;
char	input_filename[_MAX_PATH];
char	obj_filename[_MAX_PATH];
double	edge_r;
int		EdgeOutput  = 1;
int		OneFileMode = 0;
Standard_Real Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;

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
	char drive[_MAX_DRIVE];		// drive name
	char dir[_MAX_DIR];			// diretory name
	char filename[_MAX_FNAME];	// file name
	char ext[_MAX_EXT];			// extension

	//Disassemble file name to be examined and obtain drive and dir
	_splitpath(fname, drive, dir, filename, ext);

	//cout << "Drive=" << drive << endl;
	//cout << "Dir  =" << dir << endl;
	//cout << "Fname=" << filename << endl;
	//cout << "Ext  =" << ext << endl;

	//Make it an absolute path and get the real path length

	//Create a dummy file name from the obtained drive and dir
	char dummy[_MAX_PATH];
	sprintf(dummy, "%s%sdummy_%d", drive, dir, rand());

	//Convert to full path name
	char szFullPath[_MAX_PATH] = { '\0' };
	char *szFilePart;
	DWORD dwRet = GetFullPathNameA(fname, _MAX_PATH, szFullPath, &szFilePart);
	if (dwRet != 0)
	{
		//Disassemble file name to be examined and obtain drive and dir
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
		char filename2[_MAX_FNAME];
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

bool queryColor(const TopoDS_Shape& shape, Quantity_Color& color)
{
	TDF_Label L;

	color.SetValues(0.5, 0.5, 0.5, Quantity_TOC_RGB);
	if (!aColorTool->ShapeTool()->Search(shape, L))
		return false;

	if (aColorTool->GetColor(shape, XCAFDoc_ColorGen, color))
		return true;
	else if (aColorTool->GetColor(shape, XCAFDoc_ColorCurv, color))
		return true;
	else if (aColorTool->GetColor(shape, XCAFDoc_ColorSurf, color))
		return true;

	return false;
}

class CAD_reader
{
public:
	TopoDS_Shape aShape;
	TopTools_IndexedDataMapOfShapeListOfShape anEdgeMap;
	virtual int read(Standard_CString& path)
	{
		return 0;
	}

	virtual int ShapeNum()
	{
		return 0;
	}

	virtual TopoDS_Shape getShape()
	{
		return aShape;
	}
	virtual TopoDS_Shape getShape(const int i) const
	{
		return TopoDS_Shape();
	}
	virtual std::string ShapeToName(const TopoDS_Shape& theShape, float* color=NULL)
	{
		std::string name = "";
		return name;
	}
};

class Name_Color
{
public:
	std::string name;
	Color color;
};

class STEP_reader:CAD_reader
{
public:
	STEPCAFControl_Reader reader;

	std::vector<Name_Color> name_and_color;

	STEP_reader()
	{
		hApp = XCAFApp_Application::GetApplication();
		hApp->NewDocument(TCollection_ExtendedString("MDTV-CAF"), hDoc);
		//hApp->NewDocument(TCollection_ExtendedString("STEP-XCAF"), hDoc);

		aShapeTool = XCAFDoc_DocumentTool::ShapeTool(hDoc->Main());
		aColorTool = XCAFDoc_DocumentTool::ColorTool(hDoc->Main());

		STEPControl_Controller::Init();
		Interface_Static::SetIVal("read.surfacecurve.mode", 3);
		//Interface_Static::SetIVal("read.step.product.mode", 1);
		//Interface_Static::SetIVal("read.step.product.context", 1);
		//Interface_Static::SetIVal("read.step.shape.repr", 1);
		//Interface_Static::SetIVal("read.step.assembly.level", 1);
		//Interface_Static::SetIVal("read.step.shape.aspect", 1);
	}
	int read(Standard_CString& path)
	{
		if (reader.ReadFile(path) != IFSelect_RetDone) 
		{
			reader.Reader().PrintCheckLoad(Standard_True, IFSelect_ItemsByEntity);
			return 1;
		}
		reader.SetColorMode(true);
		reader.SetNameMode(true);
		reader.SetLayerMode(true);
		reader.SetPropsMode(true);
		reader.SetGDTMode(true);
		reader.SetSHUOMode(true);
		reader.SetMatMode(true);

		STEPControl_Reader reader0 = reader.Reader();
		int nr = reader0.NbRootsForTransfer();
		reader0.TransferRoots();

		reader.Transfer(hDoc);

		TDF_LabelSequence shapes;
		aShapeTool->GetShapes(shapes);
		for (int i = 1; i <= shapes.Length(); i++) {
			printf("shape %d: \n", i);
			TDF_Label label = shapes.Value(i);
			Handle(TDataStd_Name) N;
			if (label.FindAttribute(TDataStd_Name::GetID(), N)) {
				TCollection_ExtendedString name = N->Get();
				std::string s1 = TCollection_AsciiString(name).ToCString();
				printf("hey %s\n", s1.c_str());

				Handle(TNaming_NamedShape) NS;
				if (label.FindAttribute(TNaming_NamedShape::GetID(), NS))
				{
					TopoDS_Shape oneShape = NS->Get();
					if (!oneShape.IsNull())
					{
						printf("#shapeType=%d\n", oneShape.ShapeType());
						Quantity_Color color;
						if (queryColor(oneShape, color))
						{
							Name_Color c;
							c.name = s1;
							c.color = Color(color);
							name_and_color.push_back(c);
							printf("RGB:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						}
					}
				}
			}
		}

		TDF_LabelSequence all_colours;
		aColorTool->GetColors(all_colours);
		printf("Number of colours: %d\n", all_colours.Length());
		for (int i = 1; i <= all_colours.Length(); i++) {
			Quantity_Color col;
			std::stringstream col_rgb;
			aColorTool->GetColor(all_colours.Value(i), col);
			col_rgb << " : (" << col.Red() << "," << col.Green() << "," << col.Blue() << ")";
			printf("Colour %s %s\n", col.StringName(col.Name()), col_rgb.str().c_str());

		}

		printf("NbShapes:%d\n", reader0.NbShapes());
		// Convert IGES route for processing in OCC
		aShape = reader0.OneShape();
		//aShape = reader.Shape(1);

		//printf("number of roots = %d\n", nr);
		printf("NbShapes:%d\n", reader0.NbShapes());

		return 0;
	}
	inline int ShapeNum()
	{
		return reader.Reader().NbShapes();
	}

	inline TopoDS_Shape getShape()
	{
		return aShape;
	}
	inline TopoDS_Shape getShape(const int i) const
	{
		return reader.Reader().Shape(i);
	}
	std::string ShapeToName(const TopoDS_Shape& theShape, float* color = NULL)
	{
		std::string name = "";
		TDF_Label top_label = aShapeTool->FindShape(theShape);
		
		if (!top_label.IsNull())
		{
			Handle(TDataStd_Name) N;
			if (top_label.FindAttribute(TDataStd_Name::GetID(), N)) {
				TCollection_ExtendedString name_str = N->Get();
				if (!name_str.IsEmpty())
				{
					std::string s1 = TCollection_AsciiString(name_str).ToCString();
					//printf("name %s\n", s1.c_str());
					name = s1;
				}
			}
		}

		if (color)
		{
			color[0] = -1.0;
			color[1] = -1.0;
			color[2] = -1.0;
		}
		Quantity_Color qcolor;
		//if (aColorTool->IsSet(theShape, XCAFDoc_ColorSurf)||aColorTool->IsSet(theShape, XCAFDoc_ColorGen)||aColorTool->IsSet(theShape, XCAFDoc_ColorCurv))
		//{
		//	aColorTool->GetColor(theShape, XCAFDoc_ColorGen, qcolor);
		//	//printf("*rgb:%f %f %f\n", qcolor.Red(), qcolor.Green(), qcolor.Blue());
		//	if (color)
		//	{
		//		color[0] = qcolor.Red();
		//		color[1] = qcolor.Green();
		//		color[2] = qcolor.Blue();
		//	}
		//}
		if (color)
		{
			if (queryColor(theShape, qcolor))
			{
				color[0] = qcolor.Red();
				color[1] = qcolor.Green();
				color[2] = qcolor.Blue();
			}
		}
		if (color && color[0] < 0.0)
		{
			for (int i = 0; i < name_and_color.size(); i++)
			{
				if (name_and_color[i].name == name)
				{
					color[0] = name_and_color[i].color.color[0];
					color[1] = name_and_color[i].color.color[1];
					color[2] = name_and_color[i].color.color[2];
				}
			}
		}
		return name;
#if 0
		const Handle(XSControl_WorkSession)& ws = reader.Reader().WS();
		const Handle(XSControl_TransferReader)& tr = ws->TransferReader();
		const Handle(Transfer_TransientProcess)& tp = tr->TransientProcess();

		std::string name = "";
		Handle(Interface_InterfaceModel) aModel = ws->Model();
		Standard_Integer all = aModel->NbEntities();

		Handle_Standard_Transient ent = tr->EntityFromShapeResult(theShape, 1);
		if (!ent.IsNull())
		{
			ent = tr->EntityFromShapeResult(theShape, -1);
		}
		if (!ent.IsNull())
		{
			ent = tr->EntityFromShapeResult(theShape, 4);
		}
		if (!ent.IsNull()) {
			Handle(StepRepr_Representation) ReprItem =
				Handle(StepRepr_Representation)::DownCast(ent);

			if (!ReprItem.IsNull() && !ReprItem->Name().IsNull())
			{
				name = ReprItem->Name()->String().ToCString();
				return name;
			}
			if (!ReprItem.IsNull())
			{
				;
				printf("@@@@@@@@@@%d\n", ReprItem->NbItems());
			}
		}
		return name;
#endif
	}
};


class IGES_reader:CAD_reader
{
public:

	IGESCAFControl_Reader reader;

	IGES_reader()
	{
		hApp = XCAFApp_Application::GetApplication();
		hApp->NewDocument(TCollection_ExtendedString("MDTV-CAF"), hDoc);

		aShapeTool = XCAFDoc_DocumentTool::ShapeTool(hDoc->Main());
		aColorTool = XCAFDoc_DocumentTool::ColorTool(hDoc->Main());

		IGESControl_Controller::Init();
		Interface_Static::SetIVal("read.surfacecurve.mode", 3);
	}
	int read(Standard_CString& path)
	{
		if (reader.ReadFile(path) != IFSelect_RetDone) 
		{
			reader.PrintCheckLoad(Standard_True, IFSelect_ItemsByEntity);
			return 1;
		}
		reader.SetColorMode(true);
		reader.SetNameMode(true);
		reader.SetLayerMode(true);
		int nr = reader.NbRootsForTransfer();
		//reader.TransferRoots();
		reader.Transfer(hDoc);

		TDF_LabelSequence shapes;
		aShapeTool->GetShapes(shapes);
		for (int i = 1; i <= shapes.Length(); i++) {
			printf("shape %d: \n", i);
			TDF_Label label = shapes.Value(i);
			Handle(TDataStd_Name) N;
			if (label.FindAttribute(TDataStd_Name::GetID(), N)) {
				TCollection_ExtendedString name = N->Get();
				std::string s1 = TCollection_AsciiString(name).ToCString();
				printf("hey %s\n", s1.c_str());

				Handle(TNaming_NamedShape) NS;
				if (label.FindAttribute(TNaming_NamedShape::GetID(), NS))
				{
					TopoDS_Shape oneShape = NS->Get();
					if (!oneShape.IsNull())
					{
						printf("#shapeType=%d\n", oneShape.ShapeType());
						Quantity_Color color;
						if (queryColor(oneShape, color))
						{
							printf("RGB:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						}
					}
				}
			}
		}

		TDF_LabelSequence all_colours;
		aColorTool->GetColors(all_colours);
		printf("Number of colours: %d\n", all_colours.Length());
		for (int i = 1; i <= all_colours.Length(); i++) {
			Quantity_Color col;
			std::stringstream col_rgb;
			aColorTool->GetColor(all_colours.Value(i), col);
			col_rgb << " : (" << col.Red() << "," << col.Green() << "," << col.Blue() << ")";
			printf("Colour %s %s\n", col.StringName(col.Name()), col_rgb.str().c_str());

		}

		printf("NbShapes:%d\n", reader.NbShapes());
		// Convert IGES route for handling in OCC
		aShape = reader.OneShape();
		//aShape = reader.Shape(1);

		//printf("number of roots = %d\n", nr);
		printf("NbShapes:%d\n", reader.NbShapes());
		return 0;
	}

	inline int ShapeNum()
	{
		return reader.NbShapes();
	}

	inline TopoDS_Shape getShape()
	{
		return aShape;
	}
	inline TopoDS_Shape getShape(const int i) const
	{
		return reader.Shape(i);
	}
	std::string ShapeToName(const TopoDS_Shape& theShape, float* color = NULL)
	{
		std::string name = "";
		TDF_Label top_label = aShapeTool->FindShape(theShape);

		if (!top_label.IsNull())
		{
			Handle(TDataStd_Name) N;
			if (top_label.FindAttribute(TDataStd_Name::GetID(), N)) {
				TCollection_ExtendedString name_str = N->Get();
				if (!name_str.IsEmpty())
				{
					std::string s1 = TCollection_AsciiString(name_str).ToCString();
					//printf("name %s\n", s1.c_str());
					name = s1;
				}
			}
		}

		if (color)
		{
			color[0] = -1.0;
			color[1] = -1.0;
			color[2] = -1.0;
		}
		Quantity_Color qcolor;
		//if (aColorTool->IsSet(theShape, XCAFDoc_ColorSurf)||aColorTool->IsSet(theShape, XCAFDoc_ColorGen)||aColorTool->IsSet(theShape, XCAFDoc_ColorCurv))
		//{
		//	aColorTool->GetColor(theShape, XCAFDoc_ColorGen, qcolor);
		//	//printf("*rgb:%f %f %f\n", qcolor.Red(), qcolor.Green(), qcolor.Blue());
		//	if (color)
		//	{
		//		color[0] = qcolor.Red();
		//		color[1] = qcolor.Green();
		//		color[2] = qcolor.Blue();
		//	}
		//}
		if (color)
		{
			if (queryColor(theShape, qcolor))
			{
				color[0] = qcolor.Red();
				color[1] = qcolor.Green();
				color[2] = qcolor.Blue();
			}
		}
#if 0
		const Handle(XSControl_WorkSession)& ws = reader.WS();
		const Handle(XSControl_TransferReader)& tr = ws->TransferReader();

		Handle(IGESData_IGESModel) aModel = reader.IGESModel();
		//Handle(Interface_InterfaceModel) aModel = ws->Model();
		Standard_Integer all = aModel->NbEntities();

		Handle_Standard_Transient ent = tr->EntityFromShapeResult(theShape, 1);
		if (!ent.IsNull())
		{
			ent = tr->EntityFromShapeResult(theShape, -1);
		}
		if (!ent.IsNull())
		{
			ent = tr->EntityFromShapeResult(theShape, 4);
		}
		if (!ent.IsNull()) {
			Handle(IGESData_IGESEntity) ent2 =
				Handle(IGESData_IGESEntity)::DownCast(ent);

			Quantity_Color col;
			if (ent2->DefColor() == IGESData_DefValue) {
				col = IGESCAFControl::DecodeColor(ent2->RankColor());
				printf("rgb:%f %f %f\n", qcolor.Red(), qcolor.Green(), qcolor.Blue());
			}

			if (!ent2->Color().IsNull())
			{
				printf("color:%s\n", ent2->Color()->NameValue()->String().ToCString());
			}
			if (ent2->HasName())
			{
				name = ent2->NameValue()->String().ToCString();
				return name;
			}
			if (ent2->LabelDisplay()->HasName())
			{
				name = ent2->LabelDisplay()->NameValue()->String().ToCString();
				return name;
			}

			int i = aModel->Number(ent);
			if (i > 0) {
				Handle_IGESData_IGESEntity ie = aModel->Entity(i);
				if (ie->HasShortLabel())
				{
					name = ie->ShortLabel()->ToCString();
					return name;
				}
			}
		}
#endif
		return name;
	}
};
CAD_reader* cad_reader;


void dumpLabels(IGESControl_Reader& reader)
{
	const Handle(XSControl_WorkSession)& theSession = reader.WS();
	Handle(IGESData_IGESModel) theModel = reader.IGESModel();
	//Handle(Interface_InterfaceModel) theModel = theSession->Model();
	const Handle(XSControl_TransferReader)& tr = theSession->TransferReader();
	const Handle(Transfer_TransientProcess) & tp = tr->TransientProcess();

	Standard_Integer nb = theModel->NbEntities();
	for (Standard_Integer i = 1; i <= nb; i++)
	{
		Handle(IGESData_IGESEntity) ent = 
			Handle(IGESData_IGESEntity)::DownCast(theModel->Value(i));
		if (ent.IsNull()) continue;

		if (ent->HasName())
		{
			Handle(Transfer_Binder) binder = tp->Find(ent);
			if (!binder.IsNull())
			{
				TopoDS_Shape oneShape = TransferBRep::ShapeResult(binder);
				if (!oneShape.IsNull())
				{
					printf("shapeType=%d\n", oneShape.ShapeType());
				}
			}

			printf("%d %d %d ", ent->Level(), ent->DefLevel(), IGESData_DefValue);
			std::cout << ent->NameValue()->String().ToCString() << std::endl;
		}
		else
		{
		}
	}
}


void dumpLabels(const STEPControl_Reader& reader)
{
	const Handle(XSControl_WorkSession)& theSession = reader.WS();
	Handle(Interface_InterfaceModel) theModel = theSession->Model();
	const Handle(XSControl_TransferReader)& tr = theSession->TransferReader();
	const Handle(Transfer_TransientProcess) & tp = tr->TransientProcess();

	Standard_Integer nb = theModel->NbEntities();
	for (Standard_Integer i = 1; i <= nb; i++)
	{
		Handle(StepRepr_Representation) ent =
			Handle(StepRepr_Representation)::DownCast(theModel->Value(i));
		if (ent.IsNull()) continue;

		printf("%d %d ", ent->Items()->Size(), ent->NbItems());
		if (!ent->Name().IsNull())
		{
			//std::cout << ent->Name()->String().ToCString() << std::endl;
		}
		if (ent->NbItems() > 0)
		{
			//Handle(StepRepr_RepresentationItem)  br = *ent->Items()->begin();
				
			for (int i = 1; i <= ent->NbItems(); i++)
			{
				Handle(StepRepr_RepresentationItem)  br = ent->ItemsValue(i);
				if (br.IsNull()) continue;

				Handle(Transfer_Binder) binder = tp->Find(ent);
				if (!binder.IsNull())
				{
					TopoDS_Shape oneShape = TransferBRep::ShapeResult(binder);
					if (!oneShape.IsNull())
					{
						printf("shapeType=%d\n", oneShape.ShapeType());
					}
				}

				if (!br->Name().IsNull())
				{
					std::string name = br->Name()->String().ToCString();
					printf("[%s] ", name.c_str());
				}
			}
		}
		printf("\n");
	}
	printf("NbEntities:%d\n", nb);
}



void MeshShape0(const TopoDS_Shape theShape, float theDeflection, bool theForced);

void MeshShape(const TopoDS_Shape theShape,	float theDeflection,	bool theForced)
{
	int nn = 0;
	if (theDeflection <= 0)
		theDeflection = 0.01;
	//Handle(XCAFDoc_ShapeTool) aShapeTool;
	//Handle(XCAFDoc_ColorTool) aColorTool;
	TDF_Label top_label = aShapeTool->FindShape(theShape);

	if (top_label.IsNull() != Standard_True && top_label.HasChild())
	{
		TDF_ChildIterator it;

		for (it.Initialize(top_label); it.More(); it.Next())
		{
			Handle(TDataStd_Name) N;
			if (it.Value().FindAttribute(TDataStd_Name::GetID(), N)) {
				TCollection_ExtendedString name = N->Get();
				std::string s1 = TCollection_AsciiString(name).ToCString();
				//printf("name %s\n", s1.c_str());
			}

			TopExp_Explorer explorer(aShapeTool->GetShape(it.Value()), TopAbs_FACE);
			for (; explorer.More(); explorer.Next())
			{
				const TopoDS_Shape& shape = explorer.Current();

				if (shape.ShapeType() == TopAbs_FACE)
				{
					Quantity_Color localColor;
					queryColor(aShapeTool->GetShape(it.Value()), localColor);

					MeshShape0(shape, theDeflection, theForced);
				}
			}
		}
	}
	else {

		TopExp_Explorer explorer(theShape, TopAbs_FACE);
		for (; explorer.More(); explorer.Next())
		{
			const TopoDS_Shape& shape = explorer.Current();

			if (shape.ShapeType() == TopAbs_FACE)
			{
				MeshShape0(shape, theDeflection, theForced);
			}
		}

	}
}

void MeshShape0(const TopoDS_Shape theShape, float theDeflection, bool theForced)
{
	int nn = 0;
	if (theDeflection <= 0)
		theDeflection = 0.01;

	// Is shape triangulated?
	Standard_Boolean alreadymeshed = Standard_True;
	TopExp_Explorer ex;
	TopLoc_Location aLoc;
	const TopoDS_Face& aFace = TopoDS::Face(theShape);
	Handle(Poly_Triangulation) aPoly = BRep_Tool::Triangulation(aFace, aLoc);
	if (aPoly.IsNull()) {
		alreadymeshed = Standard_False;
	}

#if 10
	if (!alreadymeshed || theForced) {
		Bnd_Box B;
		BRepBndLib::Add(theShape, B);
		if (B.IsVoid())
			return; // NPAL15983 (Bug when displaying empty groups)
		Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
		B.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);

		float aDeflection = max(aXmax - aXmin, max(aYmax - aYmin, aZmax - aZmin)) * theDeflection * 4;
		//float aDeflection = ((aXmax - aXmin) + (aYmax - aYmin) + (aZmax - aZmin)) / 400.0;

		//aDeflection = 100.0*theDeflection;
		//aDeflection = 20*theDeflection;
		//aDeflection = 0.1;

		BRepTools::Clean(theShape);


		//Compute triangulation
		//BRepMesh_IncrementalMesh MESH(theShape, aDeflection);
		BRepMesh_IncrementalMesh MESH(theShape, aDeflection);
		nn++;
	}
#endif
	//printf("nn:%d\n", nn);
}

N_Cylinder IsolatedEdgeList;
N_Cylinder OneFaceEdgeList;
N_Cylinder SharedEdgeList;
N_Cylinder AllEdgeList;

void AddEdge(const TopoDS_Edge& anEdge, N_Cylinder& EdgeList)
{
	if (!BRep_Tool::Degenerated(anEdge))
	{
		Handle(Poly_PolygonOnTriangulation) aEdgePoly;
		Standard_Integer i = 1;
		Handle(Poly_Triangulation) T;
		TopLoc_Location aEdgeLoc;
		BRep_Tool::PolygonOnTriangulation(anEdge, aEdgePoly, T, aEdgeLoc, i);
		Handle(Poly_Polygon3D) P;
		if (aEdgePoly.IsNull())
			P = BRep_Tool::Polygon3D(anEdge, aEdgeLoc);

		if (P.IsNull() && aEdgePoly.IsNull())
			return;

		// Location edges
		//---------------
		gp_Trsf edgeTransf;
		Standard_Boolean isidtrsf = true;
		if (!aEdgeLoc.IsIdentity()) {
			isidtrsf = false;
			edgeTransf = aEdgeLoc.Transformation();
		}

		gp_Pnt aP1, aP2;

		if (aEdgePoly.IsNull()) {
			Standard_Integer aNbNodes = P->NbNodes();
			const TColgp_Array1OfPnt& aNodesP = P->Nodes();

			aP1 = aNodesP(1);
			aP2 = aNodesP(aNbNodes);

			for (int j = 1; j < aNbNodes; j++) {
				gp_Pnt pt1 = aNodesP(j);
				gp_Pnt pt2 = aNodesP(j + 1);

				if (!isidtrsf) {
					// apply edge transformation
					pt1.Transform(edgeTransf);
					pt2.Transform(edgeTransf);
				}

				double aCoord1[3] = { pt1.X(), pt1.Y(), pt1.Z() };
				double aCoord2[3] = { pt2.X(), pt2.Y(), pt2.Z() };

				Cylinder c1(aCoord1, aCoord2, 3.0);
				EdgeList.Add(c1);
			}
		}
		else {
			Standard_Integer aNbNodes = aEdgePoly->NbNodes();
			const TColStd_Array1OfInteger& aNodeIds = aEdgePoly->Nodes();
			const TColgp_Array1OfPnt& anId2Pnts = T->Nodes();

			aP1 = anId2Pnts(aNodeIds(1));
			aP2 = anId2Pnts(aNodeIds(aNbNodes));

			for (int j = 1; j < aNbNodes; j++) {
				Standard_Integer id1 = aNodeIds(j);
				Standard_Integer id2 = aNodeIds(j + 1);

				gp_Pnt pt1 = anId2Pnts(id1);
				gp_Pnt pt2 = anId2Pnts(id2);

				if (!isidtrsf) {
					// apply edge transformation
					pt1.Transform(edgeTransf);
					pt2.Transform(edgeTransf);
				}

				double aCoord1[3] = { pt1.X(), pt1.Y(), pt1.Z() };
				double aCoord2[3] = { pt2.X(), pt2.Y(), pt2.Z() };

				Cylinder c1(aCoord1, aCoord2, 3.0);
				EdgeList.Add(c1);
			}
		}

	}
}

double box_min[3] = { 9999999.0,9999999.0 ,9999999.0 };
double box_max[3] = { -9999999.0,-9999999.0 ,-9999999.0 };

FILE* mtlFileFp = NULL;
int count = 0;
void SetShape(const TopoDS_Shape& theShape,
	const TopTools_IndexedDataMapOfShapeListOfShape& theEdgeMap,
	bool theIsVector, int depth)
{
	if (theShape.ShapeType() == TopAbs_COMPOUND)
	{
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_COMPOUND\n");
		TopoDS_Iterator anItr(theShape);
		for (; anItr.More(); anItr.Next())
		{
			SetShape(anItr.Value(), theEdgeMap, theIsVector, depth+1);
		}
		count++;
	}
	if (theShape.ShapeType() == TopAbs_COMPSOLID)
	{
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_COMPSOLID\n");
	}
	if (theShape.ShapeType() == TopAbs_SOLID)
	{
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_SOLID\n");
		//TopExp_Explorer shape(theShape, TopAbs_SOLID);
		//for (; shape.More(); shape.Next())
	//{
	//		SetShape(shape.Current(), theEdgeMap, theIsVector, depth + 1);
	//}
		//return;
	}
	if (theShape.ShapeType() == TopAbs_SHELL)
	{
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_SHELL\n");
		//TopExp_Explorer shape(theShape, TopAbs_SOLID);
		//for (; shape.More(); shape.Next())
	//{
	//		SetShape(shape.Current(), theEdgeMap, theIsVector, depth + 1);
	//}
		//return;
	}
	if (theShape.ShapeType() == TopAbs_SHAPE)
	{
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_SHAPE\n");
	}

	switch (theShape.ShapeType()) {
	case TopAbs_WIRE: {
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_WIRE\n");
		TopExp_Explorer anEdgeExp(theShape, TopAbs_EDGE);
		for (; anEdgeExp.More(); anEdgeExp.Next()) {
			const TopoDS_Edge& anEdge = TopoDS::Edge(anEdgeExp.Current());
			if (!BRep_Tool::Degenerated(anEdge))
			{
				AddEdge(anEdge, IsolatedEdgeList);
			}
		}

		break;
	}
	case TopAbs_EDGE: {
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_EDGE\n");
		const TopoDS_Edge& anEdge = TopoDS::Edge(theShape);
		if (!BRep_Tool::Degenerated(anEdge))
		{
			AddEdge(anEdge, IsolatedEdgeList);
		}
		break;
	}
	case TopAbs_VERTEX: {
		printf("%d:%s ", depth, cad_reader->ShapeToName(theShape).c_str());
		printf("==>TopAbs_VERTEX\n");
		break;
	}
	default: {
		TopExp_Explorer aFaceExp(theShape, TopAbs_FACE);
		//TopAbs_Orientation orient = theShape.Orientation();
		//printf("%s ", cad_reader->ShapeToName(theShape).c_str());
		//printf("**%d:%s\n", depth, cad_reader->ShapeToName(theShape).c_str());

		for (; aFaceExp.More(); aFaceExp.Next())
		{
			//printf("%d:%s\n", depth, cad_reader->ShapeToName(aFaceExp.Current()).c_str());
			const TopoDS_Face& aFace = TopoDS::Face(aFaceExp.Current());
			{
				float color[3];
				//printf("%s\n", cad_reader->ShapeToName(aFace, color).c_str());
				cad_reader->ShapeToName(aFace, color);
				//printf("color:%f %f %f\n", color[0], color[1], color[2]);
				if (mtlFileFp)
				{
					if (color[0] >= 0)
					{
						char mtl[128];
						sprintf(mtl, "color_%04d", aFace.HashCode(INT_MAX));
						fprintf(mtlFileFp, "newmtl %s\n", mtl);
						fprintf(mtlFileFp, "Ka 0.0 0.0 0.0\n");
						fprintf(mtlFileFp, "Kd %.4f %.4f %.4f\n", color[0], color[1], color[2]);
						fprintf(mtlFileFp, "Ks 0.0 0.0 0.0\n");
						fprintf(mtlFileFp, "Ns 0.0 0.0 0.0\n");
						fprintf(mtlFileFp, "d 1.0\n");

						fprintf(obj_fp, "usemtl %s\n", mtl);
					}
				}

				TopLoc_Location loc = TopLoc_Location();
				Handle(Poly_Triangulation) poly = BRep_Tool::Triangulation(aFace, loc); // 三角形パッチ化
				if (poly.IsNull())
				{
					continue;
				}
				TopAbs_Orientation orient = aFace.Orientation();

				gp_Trsf myTransf;
				Standard_Boolean identity = true;
				if (!loc.IsIdentity())
				{
					identity = false;
					myTransf = loc.Transformation();
				}

				int nbNodesInFace = poly->NbNodes();
				int nbTriInFace = poly->NbTriangles();
				//printf("nbNodesInFace:%d nbTriInFace:%d\n", nbNodesInFace, nbTriInFace);

				face_num++;
				fprintf(obj_fp, "g  %d%s\n\n", face_num, cad_reader->ShapeToName(theShape).c_str());

				const Poly_Array1OfTriangle& tris = poly->Triangles();
				const TColgp_Array1OfPnt& Nodes = poly->Nodes();
				//for (Standard_Integer i = tris.Lower(); i <= tris.Upper(); i++) 
				for (Standard_Integer i = 1; i <= nbTriInFace; i++)
				{
					const Poly_Triangle& tri = tris.Value(i);
					Standard_Integer n1, n2, n3;
					if (orient == TopAbs_REVERSED)
						tri.Get(n3, n2, n1);
					else
						tri.Get(n1, n2, n3);
					gp_Pnt p1 = Nodes.Value(n1);
					gp_Pnt p2 = Nodes.Value(n2);
					gp_Pnt p3 = Nodes.Value(n3);

					if (!identity)
					{
						p1.Transform(myTransf);
						p2.Transform(myTransf);
						p3.Transform(myTransf);
					}
					// Calculate triangle normal
					gp_Vec v1(p1.X(), p1.Y(), p1.Z()), v2(p2.X(), p2.Y(), p2.Z()), v3(p3.X(), p3.Y(), p3.Z());
					gp_Vec Normal = (v2 - v1) ^ (v3 - v1);

					fprintf(obj_fp, "v %f %f %f\n", p1.X(), p1.Y(), p1.Z());
					fprintf(obj_fp, "v %f %f %f\n", p2.X(), p2.Y(), p2.Z());
					fprintf(obj_fp, "v %f %f %f\n", p3.X(), p3.Y(), p3.Z());
					//fprintf(obj_fp, "v %f %f %f %f %f %f\n", p1.X(), p1.Y(), p1.Z(), r, g, b);
					//fprintf(obj_fp, "v %f %f %f %f %f %f\n", p2.X(), p2.Y(), p2.Z(), r, g, b);
					//fprintf(obj_fp, "v %f %f %f %f %f %f\n", p3.X(), p3.Y(), p3.Z(), r, g, b);

					//fprintf(obj_fp, "vn %f %f %f\n", Normal.X(), Normal.Y(), Normal.Z());
					//fprintf(obj_fp, "vn %f %f %f\n", Normal.X(), Normal.Y(), Normal.Z());
					//fprintf(obj_fp, "vn %f %f %f\n", Normal.X(), Normal.Y(), Normal.Z());
					//fprintf(obj_fp, "f %d//%d %d//%d %d//%d\n", vindex + 1, vindex + 1, vindex + 2, vindex + 2, vindex + 3, vindex + 3);
					fprintf(obj_fp, "f %d %d %d\n", vindex + 1, vindex + 2, vindex + 3);
					vindex += 3;
					tri_num++;

					if (p1.X() < box_min[0]) box_min[0] = p1.X();
					if (p1.Y() < box_min[1]) box_min[1] = p1.Y();
					if (p1.Z() < box_min[2]) box_min[2] = p1.Z();

					if (p2.X() < box_min[0]) box_min[0] = p2.X();
					if (p2.Y() < box_min[1]) box_min[1] = p2.Y();
					if (p2.Z() < box_min[2]) box_min[2] = p2.Z();

					if (p3.X() < box_min[0]) box_min[0] = p3.X();
					if (p3.Y() < box_min[1]) box_min[1] = p3.Y();
					if (p3.Z() < box_min[2]) box_min[2] = p3.Z();

					if (p1.X() > box_max[0]) box_max[0] = p1.X();
					if (p1.Y() > box_max[1]) box_max[1] = p1.Y();
					if (p1.Z() > box_max[2]) box_max[2] = p1.Z();

					if (p2.X() > box_max[0]) box_max[0] = p2.X();
					if (p2.Y() > box_max[1]) box_max[1] = p2.Y();
					if (p2.Z() > box_max[2]) box_max[2] = p2.Z();

					if (p3.X() > box_max[0]) box_max[0] = p3.X();
					if (p3.Y() > box_max[1]) box_max[1] = p3.Y();
					if (p3.Z() > box_max[2]) box_max[2] = p3.Z();
				}
			}

			TopExp_Explorer anEdgeExp(aFaceExp.Current(), TopAbs_EDGE);
			for (; anEdgeExp.More(); anEdgeExp.Next()) {
				const TopoDS_Edge& anEdge = TopoDS::Edge(anEdgeExp.Current());
				if (!BRep_Tool::Degenerated(anEdge)) {
					// compute the number of faces
					int aNbOfFaces = theEdgeMap.FindFromKey(anEdge).Extent();
					switch (aNbOfFaces) {
					case 0:  // isolated edge
						AddEdge(anEdge, IsolatedEdgeList);
						break;
					case 1:  // edge in only one face
						AddEdge(anEdge, OneFaceEdgeList);
						break;
					default: // edge shared by at least two faces
						AddEdge(anEdge, SharedEdgeList);
						;
					}
				}
			}
		}
	}
	}
}

TopTools_IndexedDataMapOfShapeListOfShape anEdgeMap;

void OutputMesh(const TopoDS_Shape shape, char* output, FILE* outlist_fp,int& num, std::string& Name )
{
	char fname[_MAX_PATH];

	printf("[%s]\n", cad_reader->ShapeToName(shape).c_str());
	char shapeName[_MAX_PATH*2];
	//Name += "_";
	//Name += cad_reader->ShapeToName(shape);
	//printf("filename length:%d\n", Name.length());
	//if (Name.length() == 186)
	//{
	//	printf("\n");
	//}

	//strcpy(shapeName, Name.c_str());
	//if(0)
	//{
	//	char szFullPath[_MAX_PATH] = { '\0' };
	//	char *szFilePart;

	//	char fname[_MAX_PATH];
	//	sprintf(fname, "%s[]_%04d.obj", output, num);
	//	DWORD dwRet = GetFullPathNameA(fname, _MAX_PATH,szFullPath,	&szFilePart);
	//	if (dwRet != 0) {
	//		//printf("FullPath : %s\n", szFullPath);
	//		//printf("FullPath Length : %u\n", dwRet);
	//		//printf("FilePart : %s\n", szFilePart);
	//	}
	//	else {
	//		//printf("GetFullPathName failed.");
	//	}

	//	int len = dwRet + Name.length();
	//	if (len >= _MAX_PATH)
	//	{
	//		int n = _MAX_PATH - dwRet - 10;
	//		if (n <= 0)
	//		{
	//			printf("FileName Length Over!!");
	//			return;
	//		}
	//		memset(shapeName, '\0', sizeof(shapeName) / sizeof(char));
	//		strncpy(shapeName, Name.c_str(), n);
	//		strcat(shapeName, "-!etc!-");
	//	}
	//	else
	//	{
	//		strcpy(shapeName, Name.c_str());
	//	}
	//}

	if (OneFileMode <= 1)
	{
		strcpy(shapeName, Name.c_str());

		char drive[_MAX_DRIVE];		// drive name
		char dir[_MAX_DIR];			// diretory name
		char filename[_MAX_FNAME];	// file name
		char ext[_MAX_EXT];			// extension

		_splitpath(input_filename, drive, dir, filename, ext);
		
		if (OneFileMode == 1)
		{
			strcpy(shapeName, filename);
		}
		sprintf(fname, "%s[%s]_%04d.obj", output, FileNameExchange(shapeName), num);
		FileNameLengthModify(fname, _MAX_PATH - 20);//_MAX_PATH
		strcpy(obj_filename, fname);
		//printf("filename[%s]\n", fname);
		//printf("filenameLength:%d\n", strlen(fname));

		vindex = 0;
		obj_fp = fopen(fname, "w");
		if (obj_fp == NULL)
		{
			printf("WriteError->FileOpenErr:[%s]\n", fname);
			return;
		}
	}

	float color[3];
	fprintf(obj_fp, "#nodeName:%s\n", cad_reader->ShapeToName(shape, color).c_str());
	{
		////r = 255 * rand() / RAND_MAX;
		////g = 255 * rand() / RAND_MAX;
		////b = 255 * rand() / RAND_MAX;
		//r = 0.5 * 255;
		//g = 0.5 * 255;
		//b = 0.5 * 255;

		if (color[0] >= 0)
		{
			parentColor = Color(color[0] * 255,color[1] * 255, color[2] * 255);
		}

		sprintf(fname, "%s_%04d.mlt", output, num);
		if (OneFileMode <= 1) 
		{
			char drive[_MAX_DRIVE];		// drive name
			char dir[_MAX_DIR];			// diretory name
			char filename[_MAX_FNAME];	// file name
			char ext[_MAX_EXT];			// extension

			_splitpath(fname, drive, dir, filename, ext);

			//cout << "Drive=" << drive << endl;
			//cout << "Dir  =" << dir << endl;
			//cout << "Fname=" << filename << endl;
			//cout << "Ext  =" << ext << endl;

			fprintf(obj_fp, "mtllib %s%s\n", filename, ext);
		}
		FILE* fp2 = NULL;
		if (OneFileMode <= 1)
		{
			fp2 = fopen(fname, "w");
		}
		char mtl[_MAX_PATH] = { '\0' };
		if (fp2 || mtlFileFp)
		{
			if (mtlFileFp && OneFileMode <= 1) fclose(mtlFileFp);
			if (OneFileMode <= 1 ) mtlFileFp = fp2;

			sprintf(mtl, "color_%04d", num);
			fprintf(mtlFileFp, "newmtl %s\n", mtl);
			fprintf(mtlFileFp, "Ka 0.0 0.0 0.0\n");
			fprintf(mtlFileFp, "Kd %.4f %.4f %.4f\n", parentColor.color[0]/255.0, parentColor.color[1] /255.0, parentColor.color[2] /255.0);
			fprintf(mtlFileFp, "Ks 0.0 0.0 0.0\n");
			fprintf(mtlFileFp, "Ns 0.0 0.0 0.0\n");
			fprintf(mtlFileFp, "d 1.0\n");

			fprintf(obj_fp, "usemtl %s\n", mtl);
		}
	}
	SetShape(shape, anEdgeMap, false, 0);
	vtx_num += vindex;

	if (vindex > 0 && OneFileMode <= 1)
	{
		fprintf(outlist_fp, "ImportFile=%s\n", obj_filename);
	}
	if (OneFileMode == 1)
	{
		OneFileMode = 2;
	}

#if 10
	
	if (EdgeOutput && OneFileMode == 0)
	{
		char drive[_MAX_DRIVE];		// drive name
		char dir[_MAX_DIR];			// diretory name
		char filename[_MAX_FNAME];	// file name
		char ext[_MAX_EXT];			// extension

		_splitpath(obj_filename, drive, dir, filename, ext);

		char edgeFile[_MAX_PATH * 2];
		sprintf(edgeFile, "%s%s%s_EDGE(i)%s", drive, dir, filename, ext);
		FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		if (OneFileMode == 2)
		{
			sprintf(edgeFile, "%s%s%s_EDGE(i)%04d%s", drive, dir, filename, num, ext);
			FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		}
		if (IsolatedEdgeList.cylinders.size())
		{
			IsolatedEdgeList.put(edgeFile, edge_r);
			if (EdgeOutput)fprintf(outlist_fp, "ImportFile=%s\n", edgeFile);
		}


		sprintf(edgeFile, "%s%s%s_EDGE(o)%s", drive, dir, filename, ext);
		FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		if (OneFileMode == 2)
		{
			sprintf(edgeFile, "%s%s%s_EDGE(o)%04d%s", drive, dir, filename, num, ext);
			FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		}
		if (OneFaceEdgeList.cylinders.size())
		{
			OneFaceEdgeList.put(edgeFile, edge_r);
			if (EdgeOutput)fprintf(outlist_fp, "ImportFile=%s\n", edgeFile);
		}

		sprintf(edgeFile, "%s%s%s_EDGE(s)%s", drive, dir, filename, ext);
		FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		if (OneFileMode == 2)
		{
			sprintf(edgeFile, "%s%s%s_EDGE(s)%04d%s", drive, dir, filename, num, ext);
			FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH
		}
		if (SharedEdgeList.cylinders.size())
		{
			SharedEdgeList.put(edgeFile, edge_r);
			if (EdgeOutput)fprintf(outlist_fp, "ImportFile=%s\n", edgeFile);
		}
	}
	if (EdgeOutput && OneFileMode == 2)
	{
		for (int i = 0; i < IsolatedEdgeList.cylinders.size(); i++)
		{
			AllEdgeList.Add(IsolatedEdgeList.cylinders[i]);
		}
		for (int i = 0; i < OneFaceEdgeList.cylinders.size(); i++)
		{
			AllEdgeList.Add(OneFaceEdgeList.cylinders[i]);
		}
		for (int i = 0; i < SharedEdgeList.cylinders.size(); i++)
		{
			AllEdgeList.Add(SharedEdgeList.cylinders[i]);
		}
	}
	IsolatedEdgeList.cylinders.clear();
	OneFaceEdgeList.cylinders.clear();
	SharedEdgeList.cylinders.clear();
#endif
	num++;
	if (OneFileMode != 2)
	{
		fclose(obj_fp);
		obj_fp = NULL;
	}
}

void msg_loop(void)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

double TimeOutMin = -1;
int TimeOutMode = 0;
void CALLBACK TimerProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	printf("Timer End\n"); fflush(stdout);
	printf(" Time Out!!\n");
	exit(0);
	_endthread();
}

void TimeOutChecker(void* dmy)
{
	TimeOutMode = 1;	//Timer start
	MSG msg;

	SetTimer(NULL, 0, 1000*60* TimeOutMin, (TIMERPROC)TimerProc);
	printf("Timer Start\n");
	int i = 0;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//if (msg.message == WM_TIMER)
		//{
		//	TimeOutMode = -1;	//Time out!!
		//	//printf(" Time Out!!\n");
		//	//exit(0);
		//	_endthread();
		//	break;
		//}
		if (TimeOutMode == 2)	// Timer stop
		{
			_endthread();
			TimeOutMode = -2;
			break;
		}
	}
}

int main(int argc, char** argv)
{
	printf("This program uses Open CASCADE Technology software library.(OCCT)\n");
	printf("OCCT Copyright (c) 1992-1999 Matra Datavision\n");
	printf("OCCT Copyright (c) 1999-2014 OPEN CASCADE SAS\n");

	float Deflection = 0.005;

	IGES_reader* iges_reader = NULL;
	STEP_reader* step_reader = NULL;

	Standard_CString path = "";

	if (argc < 2)
	{
		printf("igecnv.exe IGESfile -o output.obj [-edge 0|1 -one 0|1 -timeout min\n");
		return -1;
	}

	char* cadFile = NULL;
	char* output = NULL;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			output = argv[i + 1];
			i++;
			continue;
		}
		if (strcmp(argv[i], "-edge") == 0)
		{
			EdgeOutput = atoi(argv[i + 1]);
			i++;
			continue;
		}
		if (strcmp(argv[i], "-one") == 0)
		{
			OneFileMode = atoi(argv[i + 1]);
			i++;
			continue;
		}
		if (strcmp(argv[i], "-timeout") == 0)
		{
			TimeOutMin = atof(argv[i + 1]);
			i++;
			continue;
		}
		else
		{
			cadFile = argv[i];
			strcpy(input_filename, cadFile);
		}
	}
	if (cadFile == NULL || output == NULL)
	{
		return -1;
	}

	Standard_CString fileName = cadFile;

	int len = strlen(cadFile);
	char exten[4];
	strcpy(exten, cadFile + (len - 4));
	if (!strcmp(exten, "iges") || !strcmp(exten, ".igs") || !strcmp(exten, "IGES") || !strcmp(exten, ".IGS"))
	{
		iges_reader = new IGES_reader;
		cad_reader = (CAD_reader*)iges_reader;
	}
	if (!strcmp(exten, "step") || !strcmp(exten, ".stp") || !strcmp(exten, "STEP") || !strcmp(exten, ".STP"))
	{
		step_reader = new STEP_reader;
		cad_reader = (CAD_reader*)step_reader;
	}
	if (step_reader == NULL && iges_reader == NULL)
	{
		printf("filename.??? <-- iges ? step ?\n");
		return -2;
	}

	if (cad_reader->read(fileName) != 0)
	{
		return -3;
	}

	//if (iges_reader) dumpLabels(((IGES_reader*)cad_reader)->reader);
	//if (step_reader) dumpLabels(((STEP_reader*)cad_reader)->reader.Reader());

	//reader = iges_reader.reader;
	int N = cad_reader->ShapeNum();
	printf("N=%d\n", N);

	printf("Polygonizing start...\n"); fflush(stdout);
	//MeshShape(cad_reader->getShape(), Deflection, true);
	for (int i = 1; i <= N; i++)
	{
		unsigned long thread_id = 0;
		if (TimeOutMin > 0 ) _beginthread(TimeOutChecker, 0, (void *)&thread_id);
		//printf("%s\n", cad_reader->ShapeToName(cad_reader->getShape(i)).c_str());
		MeshShape(cad_reader->getShape(i), Deflection, true);

		if (TimeOutMin > 0)
		{
			TimeOutMode = 2;
			msg_loop();
		}
	}

	TopExp::MapShapesAndAncestors(cad_reader->getShape(), TopAbs_EDGE, TopAbs_FACE, anEdgeMap);
	//call method to set the shape
	printf("Polygonizing end\n"); fflush(stdout);


	Bnd_Box B;
	for (int i = 1; i <= N; i++)
	{
		BRepBndLib::Add(cad_reader->getShape(i), B);
		if (B.IsVoid())
			continue;
	}
	B.Get(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);
	edge_r = (Xmax - Xmin)*(Xmax - Xmin) + (Ymax - Ymin)*(Ymax - Ymin) + (Zmax - Zmin)*(Zmax - Zmin);
	edge_r = sqrt(edge_r)*0.00025;

	char fname[_MAX_PATH];

	sprintf(fname, "%s_outputList.log", output);
	FILE* outlist_fp = fopen(fname, "w");

	//fp = fopen("test.obj", "w");

#if 10
	std::string Name = "";
	int num = 1;

	std::vector<TopoDS_Shape> SolidList;
	std::vector<std::string> SolidNameList;
	std::vector<Color> SolidColorList;

	std::vector<TopoDS_Shape> CompShapeList;
	std::vector<std::string> CompShapeNameList;
	std::vector<Color> CompShapeColorList;

	int n = 0;
	for (int i = 1; i <= N; i++)
	{
		float r = -1.0f, g = -1.0f, b = -1.0f;
		Color rgb;
		std::string name = cad_reader->ShapeToName(cad_reader->getShape(i), rgb.color);
		printf("=>%s\n", name.c_str());
		Quantity_Color color;
		if (queryColor(cad_reader->getShape(i), color))
		{
			printf("=>rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
			r = color.Red();
			g = color.Green();
			b = color.Blue();
		}
		else
		{
			if (rgb.color[0] >= 0)
			{
				r = rgb.color[0];
				g = rgb.color[1];
				b = rgb.color[2];
			}
		}

		TopoDS_Iterator shape(cad_reader->getShape(i));
		for (; shape.More(); shape.Next())
		{
			Color rgb(r, g, b);
			if (shape.Value().ShapeType() == TopAbs_SOLID)
			{
				SolidColorList.push_back(rgb);

				Color rgb_tmp;
				SolidList.push_back(shape.Value());
				std::string name2 = cad_reader->ShapeToName(SolidList[SolidList.size() - 1], rgb_tmp.color);
				if (name != "" && name2 != "")
				{
					SolidNameList.push_back(name + ";" + name2);
				}
				else
				{
					if (name != "") SolidNameList.push_back(name);
					if (name2 != "") SolidNameList.push_back(name2);
					if (name == "" && name2 == "") SolidNameList.push_back("");
				}
				printf(":[%s]\n", SolidNameList.back().c_str());
				Quantity_Color color;
				if (queryColor(shape.Value(), color))
				{
					printf(":rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
					SolidColorList.back() = Color(color);
				}
				else
				{
					if (r >= 0) SolidColorList.back() = Color(r, g, b);
					else if (rgb_tmp.color[0] >= 0) SolidColorList.back() = Color(rgb_tmp);
				}
			}
			else if (shape.Value().ShapeType() == TopAbs_COMPOUND)
			{
				CompShapeColorList.push_back(rgb);

				Color rgb_tmp;
				CompShapeList.push_back(shape.Value());
				std::string name2 = cad_reader->ShapeToName(CompShapeList.back(), rgb_tmp.color);
				if (name != "" && name2 != "")
				{
					CompShapeNameList.push_back(name + ";" + name2);
				}
				else
				{
					if (name != "") CompShapeNameList.push_back(name);
					if (name2 != "") CompShapeNameList.push_back(name2);
					if (name == "" && name2 == "") CompShapeNameList.push_back("");
				}
				printf("*[%s]\n", CompShapeNameList.back().c_str());
				Quantity_Color color;
				if (queryColor(shape.Value(), color))
				{
					printf("*rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
					CompShapeColorList.back() = Color(color);
				}
				else
				{
					if (r >= 0) CompShapeColorList.back() = Color(r, g, b);
					else if (rgb_tmp.color[0] >= 0) CompShapeColorList.back() = Color(rgb_tmp);
				}
			}
			else
			{
				//printf("shape.Value().ShapeType()=%d\n", shape.Value().ShapeType());
			}
			n++;
		}
		printf("n=%d\n", n);
	}

	std::vector<TopoDS_Shape> CompShapeListOrg;
	std::vector<std::string> CompShapeNameListOrg;
	std::vector<Color> CompShapeColorListOrg;
	if (SolidNameList.size() == 0)
	{
		CompShapeListOrg = CompShapeList;
		CompShapeNameListOrg = CompShapeNameList;
		CompShapeColorListOrg = CompShapeColorList;
	}

	while (CompShapeList.size())
	{
		std::vector<TopoDS_Shape> CompShapeList2;
		std::vector<std::string> CompShapeNameList2;
		std::vector<Color> CompShapeColorList2;
		int n = 0;
		for (int i = 0; i < CompShapeList.size(); i++)
		{
			std::string name = CompShapeNameList[i];
			Color rgb = CompShapeColorList[i];

			TopoDS_Iterator shape(CompShapeList[i]);
			for (; shape.More(); shape.Next())
			{
				if (shape.Value().ShapeType() == TopAbs_SOLID)
				{
					SolidColorList.push_back(rgb);

					Color rgb_tmp;
					SolidList.push_back(shape.Value());
					std::string name2 = cad_reader->ShapeToName(SolidList.back(), rgb_tmp.color);
					if (name != "" && name2 != "")
					{
						SolidNameList.push_back(name + ";" + name2);
					}
					else
					{
						if (name != "") SolidNameList.push_back(name);
						if (name2 != "") SolidNameList.push_back(name2);
						if (name == "" && name2 == "") SolidNameList.push_back("");
					}
					printf("::[%s]\n", SolidNameList.back().c_str());
					Quantity_Color color;
					if (queryColor(shape.Value(), color))
					{
						printf("::rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						SolidColorList.back() = Color(color);
					}
					else
					{
						if (rgb.color[0] >= 0) SolidColorList.back() = rgb;
						else if (rgb_tmp.color[0] >= 0) SolidColorList.back() = Color(rgb_tmp);
					}
				}
				else if (shape.Value().ShapeType() == TopAbs_COMPOUND)
				{
					CompShapeColorList2.push_back(rgb);

					Color rgb_tmp;
					CompShapeList2.push_back(shape.Value());
					std::string name2 = cad_reader->ShapeToName(CompShapeList2.back(), rgb_tmp.color);
					if (name != "" && name2 != "")
					{
						CompShapeNameList2.push_back(name + ";" + name2);
					}
					else
					{
						if (name != "") CompShapeNameList2.push_back(name);
						if (name2 != "") CompShapeNameList2.push_back(name2);
						if (name == "" || name2 == "") CompShapeNameList2.push_back("");
					}
					printf("**[%s]\n", CompShapeNameList2.back().c_str());
					Quantity_Color color;
					if (queryColor(shape.Value(), color))
					{
						printf("**rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						CompShapeColorList2.back() = Color(color);
					}
					else
					{
						if (rgb.color[0] >= 0) CompShapeColorList2.back() = rgb;
						else if (rgb_tmp.color[0] >= 0) CompShapeColorList2.back() = Color(rgb_tmp);
					}
				}
				else
				{
					printf("shape.Value().ShapeType()=%d\n", shape.Value().ShapeType());
				}
				n++;
			}
			printf("n=%d\n", n);
		}
		CompShapeList = CompShapeList2;
		CompShapeNameList = CompShapeNameList2;
		CompShapeColorList = CompShapeColorList2;
	}
	printf("Solid Num:%d\n", SolidList.size());


#if 10
	if (1)
	{
		// Free Shells
		for (int i = 1; i <= N; i++)
		{
			float r = -1.0f, g = -1.0f, b = -1.0f;
			Color rgb;
			std::string name = cad_reader->ShapeToName(cad_reader->getShape(i), rgb.color);
			printf("=>%s\n", name.c_str());
			Quantity_Color color;
			if (queryColor(cad_reader->getShape(i), color))
			{
				printf("=>rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
				r = color.Red();
				g = color.Green();
				b = color.Blue();
			}
			else
			{
				if (rgb.color[0] >= 0)
				{
					r = rgb.color[0];
					g = rgb.color[1];
					b = rgb.color[2];
				}
			}
			TopExp_Explorer exp1;
			for (exp1.Init(cad_reader->getShape(i), TopAbs_SHELL, TopAbs_SOLID); exp1.More(); exp1.Next())
			{
				Color rgb(r, g, b);
				{
					SolidColorList.push_back(rgb);

					Color rgb_tmp;
					SolidList.push_back(exp1.Current());
					std::string name2 = cad_reader->ShapeToName(SolidList[SolidList.size() - 1], rgb_tmp.color);
					if (name != "" && name2 != "")
					{
						SolidNameList.push_back(name + ";" + name2);
					}
					else
					{
						if (name != "") SolidNameList.push_back(name);
						if (name2 != "") SolidNameList.push_back(name2);
						if (name == "" && name2 == "") SolidNameList.push_back("");
					}
					printf(":[%s]\n", SolidNameList.back().c_str());
					Quantity_Color color;
					if (queryColor(exp1.Current(), color))
					{
						printf(":rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						SolidColorList.back() = Color(color);
					}
					else
					{
						if (r >= 0) SolidColorList.back() = Color(r, g, b);
						else if (rgb_tmp.color[0] >= 0) SolidColorList.back() = Color(rgb_tmp);
					}
				}
			}
		}
	}
#endif

#if 10
	if (SolidList.size() == 0)
	{
		float r = -1.0f, g = -1.0f, b = -1.0f;
		// Free Face
		for (int i = 1; i <= N; i++)
		{
			Color rgb;
			std::string name = cad_reader->ShapeToName(cad_reader->getShape(i), rgb.color);
			printf("=>%s\n", name.c_str());
			Quantity_Color color;
			if (queryColor(cad_reader->getShape(i), color))
			{
				printf("=>rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
				r = color.Red();
				g = color.Green();
				b = color.Blue();
			}
			else
			{
				if (rgb.color[0] >= 0)
				{
					r = rgb.color[0];
					g = rgb.color[1];
					b = rgb.color[2];
				}
			}
			TopExp_Explorer exp1;
			for (exp1.Init(cad_reader->getShape(i), TopAbs_FACE, TopAbs_COMPOUND); exp1.More(); exp1.Next())
			{
				Color rgb(r, g, b);
				{
					SolidColorList.push_back(rgb);

					Color rgb_tmp;
					SolidList.push_back(exp1.Current());
					std::string name2 = cad_reader->ShapeToName(SolidList[SolidList.size() - 1], rgb_tmp.color);
					if (name != "" && name2 != "")
					{
						SolidNameList.push_back(name + ";" + name2);
					}
					else
					{
						if (name != "") SolidNameList.push_back(name);
						if (name2 != "") SolidNameList.push_back(name2);
						if (name == "" && name2 == "") SolidNameList.push_back("");
					}
					printf(":[%s]\n", SolidNameList.back().c_str());
					Quantity_Color color;
					if (queryColor(exp1.Current(), color))
					{
						printf(":rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
						SolidColorList.back() = Color(color);
					}
					else
					{
						if (r >= 0) SolidColorList.back() = Color(r, g, b);
						else if (rgb_tmp.color[0] >= 0) SolidColorList.back() = Color(rgb_tmp);
					}
				}
			}
		}
	}
#endif

#if 10
	// Free Wires
	for (int i = 1; i <= N; i++)
	{
		float r = -1.0f, g = -1.0f, b = -1.0f;
		Color rgb;
		std::string name = cad_reader->ShapeToName(cad_reader->getShape(i), rgb.color);
		printf("=>%s\n", name.c_str());
		Quantity_Color color;
		if (queryColor(cad_reader->getShape(i), color))
		{
			printf("=>rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
			rgb = Color(color);
		}



		int n = 1;
		TopExp_Explorer exp1;
		for (exp1.Init(cad_reader->getShape(i), TopAbs_WIRE, TopAbs_FACE); exp1.More(); exp1.Next())
		{
			std::string wire_name = "";
			Color rgb(r, g, b);
			Color rgb_tmp;
			std::string name2 = cad_reader->ShapeToName(exp1.Current(), rgb_tmp.color);
			if (name != "" && name2 != "")
			{
				wire_name = name + ";" + name2;
			}
			else
			{
				if (name != "") wire_name = name;
				if (name2 != "") wire_name = name2;
				if (name == "" && name2 == "") wire_name = "free_wire";
			}
			Quantity_Color color;
			if (queryColor(exp1.Current(), color))
			{
				printf(":rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
				rgb = Color(color);
			}
			else
			{
				if (rgb_tmp.color[0] >= 0) rgb = Color(rgb_tmp);
			}

			N_Cylinder  FreeWiresList;
			TopExp_Explorer anEdgeExp(exp1.Current(), TopAbs_EDGE);
			for (; anEdgeExp.More(); anEdgeExp.Next()) {
				const TopoDS_Edge& anEdge = TopoDS::Edge(anEdgeExp.Current());
				if (!BRep_Tool::Degenerated(anEdge))
				{
					AddEdge(anEdge, FreeWiresList);
				}
			}

			char fname[_MAX_PATH * 2];
			sprintf(fname, "%s[%s]%d_%d.obj", output, wire_name.c_str(), i, n);
			FileNameLengthModify(fname, _MAX_PATH - 20);//_MAX_PATH

			int icolor[3] = { 255 * rgb.color[0], 255 * rgb.color[1], 255 * rgb.color[2] };
			if (FreeWiresList.cylinders.size() > 0 ) FreeWiresList.put(fname, edge_r, icolor);
			n++;
		}
	}
#endif

#if 10
	// Free Edges
	for (int i = 1; i <= N; i++)
	{
		float r = -1.0f, g = -1.0f, b = -1.0f;
		Color rgb;
		std::string name = cad_reader->ShapeToName(cad_reader->getShape(i), rgb.color);
		printf("=>%s\n", name.c_str());
		Quantity_Color color;
		if (queryColor(cad_reader->getShape(i), color))
		{
			printf("=>rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
			rgb = Color(color);
		}



		int n = 1;
		TopExp_Explorer exp1;
		for (exp1.Init(cad_reader->getShape(i), TopAbs_EDGE, TopAbs_WIRE); exp1.More(); exp1.Next())
		{
			std::string wire_name = "";
			Color rgb(r, g, b);
			Color rgb_tmp;
			std::string name2 = cad_reader->ShapeToName(exp1.Current(), rgb_tmp.color);
			if (name != "" && name2 != "")
			{
				wire_name = name + ";" + name2;
			}
			else
			{
				if (name != "") wire_name = name;
				if (name2 != "") wire_name = name2;
				if (name == "" && name2 == "") wire_name = "free_edge";
			}
			Quantity_Color color;
			if (queryColor(exp1.Current(), color))
			{
				printf(":rgb:%f %f %f\n", color.Red(), color.Green(), color.Blue());
				rgb = Color(color);
			}
			else
			{
				if (rgb_tmp.color[0] >= 0) rgb = Color(rgb_tmp);
			}

			N_Cylinder  FreeEdgeList;
			const TopoDS_Edge& anEdge = TopoDS::Edge(exp1.Current());
			if (!BRep_Tool::Degenerated(anEdge))
			{
				AddEdge(anEdge, FreeEdgeList);
			}


			char fname[_MAX_PATH * 2];
			sprintf(fname, "%s[%s]%d_%d.obj", output, wire_name.c_str(), i, n);
			FileNameLengthModify(fname, _MAX_PATH - 20);//_MAX_PATH

			int icolor[3] = { 255 * rgb.color[0], 255 * rgb.color[1], 255 * rgb.color[2] };
			if (FreeEdgeList.cylinders.size() > 0) FreeEdgeList.put(fname, edge_r, icolor);
			n++;
		}
	}
#endif

	if (SolidList.size() == 0)
	{
		SolidList = CompShapeListOrg;
		SolidNameList = CompShapeNameListOrg;
		SolidColorList = CompShapeColorListOrg;
	}

	if (SolidList.size() == 0 && CompShapeList.size() == 0)
	{
		for (int i = 1; i <= N; i++)
		{
			Color rgb;
			SolidList.push_back(cad_reader->getShape(i));
			SolidNameList.push_back(cad_reader->ShapeToName(SolidList[SolidList.size() - 1], rgb.color));
			SolidColorList.push_back(rgb);
		}
	}
	//SetShape(aShape, anEdgeMap, false);
	for (int i = 0; i < SolidList.size(); i++)
	{
		parentColor = Color();
		int n = 0;
		printf("[%s]\n", cad_reader->ShapeToName(SolidList[i]).c_str());
		Name = SolidNameList[i];
		std::string Name2 = Name;

		parentColor = Color(SolidColorList[i].color[0] * 255,SolidColorList[i].color[1] * 255,SolidColorList[i].color[2] * 255);
		if (parentColor.color[0] < 0)
		{
			//r = 255 * rand() / RAND_MAX;
			//g = 255 * rand() / RAND_MAX;
			//b = 255 * rand() / RAND_MAX;
			parentColor = Color(0.75 * 255, 0.75 * 255, 0.75 * 255);
		}
		//TopExp_Explorer shape(ShapeList[i], TopAbs_COMPOUND);
		//for (n = 0; shape.More(); shape.Next()) n++;
		//if (n)
		//{
		//	shape.ReInit();
		//	for (; shape.More(); shape.Next())
		//	{
		//		vindex = 0;
		//		OutputMesh(shape.Current(), EdgeList, OneFaceEdgeListAll,output, outlist_fp, num, Name);
		//	}
		//}

		if (n == 0)
		{
			TopExp_Explorer shape(SolidList[i], TopAbs_SOLID);
			for (n = 0; shape.More(); shape.Next()) n++;
			if (n)
			{
				shape.ReInit();
				for (; shape.More(); shape.Next())
				{
					std::string name = cad_reader->ShapeToName(shape.Current());
					if (name != "")
					{
						Name += ";";
						Name += name;
					}
					//vindex = 0;
					OutputMesh(shape.Current(), output, outlist_fp, num, Name);
					Name = Name2;
				}
			}
		}
		if (n == 0)
		{
			TopExp_Explorer shape(SolidList[i], TopAbs_SHELL);
			for (n = 0; shape.More(); shape.Next()) n++;
			if (n)
			{
				shape.ReInit();
				for (; shape.More(); shape.Next())
				{
					//vindex = 0;
					OutputMesh(shape.Current(), output, outlist_fp, num, Name);
					Name = Name2;
				}
			}
		}

		if ( n == 0)
		{
			//vindex = 0;
			OutputMesh(SolidList[i],  output, outlist_fp, num, Name);
			Name = Name2;
		}
	}

	printf("face=%d vertex=%d triangle=%d\n", face_num, vtx_num, tri_num);
	//printf("@@@@@@@@@\n");
	//fclose(fp);
#endif
	if (obj_fp) fclose(obj_fp);

	if (OneFileMode == 2 && EdgeOutput)
	{
		char drive[_MAX_DRIVE];		// drive name
		char dir[_MAX_DIR];			// diretory name
		char filename[_MAX_FNAME];	// file name
		char ext[_MAX_EXT];			// extension

		_splitpath(input_filename, drive, dir, filename, ext);

		char edgeFile[_MAX_PATH * 2];
		sprintf(edgeFile, "%s%s%s_EDGE%s", drive, dir, filename, ".obj");
		FileNameLengthModify(edgeFile, _MAX_PATH);//_MAX_PATH

		AllEdgeList.put(edgeFile, edge_r);
		if (EdgeOutput)fprintf(outlist_fp, "ImportFile=%s\n", edgeFile);
	}
	printf("count:%d\n", count);
	if (outlist_fp) fclose(outlist_fp);

	return 0;
}