#include <iostream>
// Gdal includes that are needed for this tutorial
#include <gdal.h>
#include <gdal_priv.h>
#include <ogrsf_frmts.h>

using namespace std;

int main(int argc, char** argv )
{
	float xright,ybottom,x,y;
	if (argc > 1)
	{
		// Now getting information for this dataset
		cout << "File Inputed: " << argv[1] << endl;

		// Lets open a Tif
		GDALDataset *poDataset;

		// Register all gdal drivers -- load every possible driver gdal has
		GDALAllRegister();

		// lets load a "dataset" which is gdal terminology for your data
		poDataset = (GDALDataset*) GDALOpen(argv[1], GA_ReadOnly);

		// Get width and height of this dataset

		int width = GDALGetRasterXSize(poDataset);
		int height = GDALGetRasterYSize(poDataset);
		cout << "Data size: " << width << " " << height << endl;

		// Get projection information of this dataset
		string proj;
		proj = string(poDataset->GetProjectionRef());
		cout << "Projection: " << proj << endl;

		// get geo transform for this dataset
		double adfGeoTransform[6];
		if ( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
		{
			
			
		    cout << "printf( Origin = (%.6f,%.6f)\n,adfGeoTransform[0],adfGeoTransform[3] )" << endl;	
			printf( "Origin = (%.6f,%.6f)\n", adfGeoTransform[0], adfGeoTransform[3] );
			
            cout << "printf( Pixel Size = (%.6f,%.6f)\n,adfGeoTransform[1],adfGeoTransform[5])" << endl;
			printf( "Pixel Size = (%.6f,%.6f)\n", adfGeoTransform[1], adfGeoTransform[5] );
			
			
			x = adfGeoTransform[0];
			y = adfGeoTransform[3];
			xright = x + adfGeoTransform[1] * width;
			ybottom = y + adfGeoTransform[5] * height;
			cout << "East: " << x << " West: " << xright << " North: " << y << " South: " << ybottom << endl; 
			cout << "Pixel Size(x y): " << adfGeoTransform[1] << " " << adfGeoTransform[5] << endl;
			
			
			cout << "adfGeoTransform[0]:  x corrdinate (East) " << adfGeoTransform[0] << endl;
			cout << "adfGeoTransform[1]:  pixel size x " << adfGeoTransform[1] << endl;
			cout << "adfGeoTransform[2]:  NULL " << adfGeoTransform[2] << endl;
			cout << "adfGeoTransform[3]:  y corrdinate (North) " << adfGeoTransform[3] << endl;
			cout << "adfGeoTransform[4]:  NULL " << adfGeoTransform[4] << endl;
			cout << "adfGeoTransform[5]:  pixel size y " << adfGeoTransform[5] << endl;
			
			
		}
		else
		{
			return 1;
		}
		
		
		
		//pDriverVRT = GetGDALDriverManager()->GetDriverByName(formatVRT);
        
		const char *formatTIFF = "GTiff";
		//char *filenameTIFF= "orignaltiff.tif";
		const char *output ="output_file.tif";
		
		GDALDataset *pTiffDS;

		
		GDALDriver *pDriverTIFF, pDriverVRT;
	    pDriverTIFF = GetGDALDriverManager()->GetDriverByName(formatTIFF);

		//pTiffDS = pDriverTIFF->Create(filenameTIFF, cols, rows, bands, GDT_Float32, NULL);
		//pTiffDS = pDriverTIFF->Create(filenameTIFF, width, height, 1, GDT_Float32, NULL);
		pTiffDS = pDriverTIFF->Create(output, width, height, 1, GDT_Float64, NULL);


		//pVrtDS = pDriverVRt->CreateCopy(filenameVRT, pTiffDS, FALSE, NULL, NULL, NULL);
		
	  int ncols=width;
      int nrows=height;

	  
	  float *rowOld = (float*) CPLMalloc(sizeof(float)*ncols);
      float *rowNew = (float*) CPLMalloc(sizeof(float)*ncols);
 
 
      /* iterate through all pixels in 2D dBZ
      * reflectivity array and assign values into rowBuff
      * array. Then write each row into Geotiff object.
      */
	  
	  
	  int noData = poDataset->GetRasterBand(1)->GetNoDataValue();

 
    for(int row=0; row<nrows; row++) 
	{
		
		poDataset->GetRasterBand(1)->RasterIO(GF_Read,0,row,ncols,1,rowOld,ncols,1,GDT_Float32,0,0);

        for(int col=0; col<ncols; col++) 
		{
         //   rowBuff[col] = (float)dbz[row][col];
		 
		     if (rowOld[col]==noData)
		     {
			    rowNew[col]=noData;
		     }
			 
		    else
			 
			 {
				rowNew[col]=rowOld[col]+10;
			 }
		 
		 
		 
        }
		
        pTiffDS->GetRasterBand(1)->RasterIO(GF_Write,0,row,ncols,1,rowNew,ncols,1,GDT_Float32,0,0);
    }


   //geotiffDataset = driverGeotiff->Create(tiffname.c_str(),ncols,nrows,1,GDT_Float32,NULL);
   //geotiffDataset->SetGeoTransform(transform);
   //geotiffDataset->SetProjection(gribDataset->GetProjectionRef());

   CPLFree(rowOld);
   CPLFree(rowNew);

   GDALClose(poDataset);
   GDALClose(pTiffDS);
   
   //GDALDestroyDriverManager();


	
	}
	return 0;

}
