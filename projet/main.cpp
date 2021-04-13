#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <opencv2/core/types_c.h>
#include <cstdlib>
#include <math.h>
//#include "json.hpp"
//#include "rapidjson/document.h"
#include <fstream>
//#include <json/value.h>
using namespace cv;
using namespace std;
//using json=nlohmann::json;
//using namespace rapidjson;


//faire une seuillage 
void seuillage(int n, Mat image, Mat aide){

    for( int i = 0 ; i < image.rows; i++){
        for(int j =0 ; j < image.cols;j++){
            if(image.at<uchar>(i,j)>n){
                // question 1
                aide.at<uchar>(i,j)=255;
            }

        }
    }

}



//faire un histogramme
void histogramme(Mat image,int hist[], int taille){

    for(int i =0 ; i < taille; i++){
        hist[i]=0;
    }

    for (int i =0 ; i < image.rows; i++){
        for (int j = 0; j < image.cols; j++){
            hist[image.at<Vec3b>(i,j)[0]]++;
        }
      

    }

    
}

//histogramme cumulé 
void histoCumule(double cumule[],int hist[],int taille,Mat img){
     for(int i =0 ; i < taille; i++){
        cumule[i]=0;
    }
    for(int i =0; i < taille; i++){
        
        for(int j=0; j <i ;  j++){
            cumule[i]+=hist[j];
        }
        cumule[i]=cumule[i]/(img.rows*img.cols);
    }
}

int max1(int tab[], int taille){

    int max=tab[0];
    for (int i = 0 ; i < taille ; i ++){
        if(max <tab[i]){
            max =tab[i];
        }

    }
    return max;
}

void afficheTab(int tab[]){

     for( int i = 0 ; i < 256 ; i ++){
        cout << " tab "<< i <<" : " <<tab[i]<<endl;

    }

}

//afficher une tableau double 
void afficheTab(double tab[]){

     for( int i = 0 ; i < 256 ; i ++){
        //cout << " tab "<< i <<" : " <<tab[i]<<endl;
        cout<<tab[i]<<endl;
    }

}


int max2(int tab[], int taille){

    int max=tab[0];
    int m=0;
    int i=255;
    bool choix =true;
    while(choix){
        if(tab[i]!=0){
            m=i;
            choix=false;
        }
        i--;
    }



   /* for (int i = 0 ; i < taille ; i ++){
        if(max <tab[i]){
            max =tab[i];
            m=i;
        }

    }
    */
    return m;
}
int minImage (int tab[], int taille, int tailleImage){

    int min=tailleImage;
    int m =0;
    bool  choix=true;
    int i =0;
    while(choix){

        if(tab[i]!=0){
            m=i;
            choix=false;
        }
        i++;

    }
/*
    for (int i = 0 ; i < taille ; i ++){
        if(tab[i]!=0){
            if(min >tab[i]){
                min=tab[i];
                m=i;
        }
        }
        

    }
    */
    return m;
}

int somme( int hist [],int taille){
    int res=0;
    for( int i =0; i < taille; i++){
        res+=hist[i];
    }
    return res;
}

//Convolution 
/*
Mat  convolution(Mat img,float matrice[][], int taille){
   Mat res =Mat::zeros(img.size(),CV_8UC1);
   mat aide;
   int t = taille%2;
    if(t!=0){
    Mat aide=Mat::zeros(img.rows+(t),img.cols+(t),CV_8UC1);
    }else{
            Mat aide=Mat::zeros(img.size(),CV_8UC1);

    }
    
    for(int i=0; i<img.rows;i++){
        for(int j=0;j<img.cols;j++){

        }
    }


    return res;

}
*/



// cumuluation d'un tableau de probabilite avec le debut et la fin du calcul
double cumulation(double prob[], int debut, int fin){
    double res=0;
    for(debut; debut <=fin;++debut){
        res+=prob[debut];
    }
   
    return res;

}


// Faire une somme 

double somme(double w[],int debut,int fin){
    double res=0;
    for(debut; debut <=fin;++debut){
        res+=w[debut];
    }

    return res;
}



//trouver le minimum du tableau 
double minimum(double varIntrClass[], int taille){
  double min = varIntrClass[0];
  int indice=0;

    for(int i =0 ; i < taille;++i){
        if(varIntrClass[i]<min){
            min =varIntrClass[i];
            indice=i;
        }
    }

    return indice;

}


int main(){
    /*******************avoir une image ************************/
    Mat img ;
   // img = imread("/home/aurelie/Images/projet image/image5.png",IMREAD_COLOR);
 //  img = imread("/home/aurelie/Images/projet image/image2.jpg",IMREAD_COLOR);
    img = imread("/home/aurelie/Images/projet image/Image7.png",IMREAD_COLOR);
    imshow("image",img);
   //waitKey(0);
    /**************Avoir une image en niveau de gris ****************/
    Mat nivGris(img.size(),CV_8UC1);
    for(int i =0 ; i < img.rows; i++){
        for(int j=0 ; j < img.cols; j++){
            nivGris.at<uchar>(i,j)=(img.at<Vec3b>(i,j)[0]+img.at<Vec3b>(i,j)[1]+img.at<Vec3b>(i,j)[2])/3;
          /* nivGris.at<Vec3b>(i,j)[0]=(img.at<Vec3b>(i,j)[0]+img.at<Vec3b>(i,j)[1]+img.at<Vec3b>(i,j)[2])/3;
            nivGris.at<Vec3b>(i,j)[1]=(img.at<Vec3b>(i,j)[0]+img.at<Vec3b>(i,j)[1]+img.at<Vec3b>(i,j)[2])/3;
            nivGris.at<Vec3b>(i,j)[2]=(img.at<Vec3b>(i,j)[0]+img.at<Vec3b>(i,j)[1]+img.at<Vec3b>(i,j)[2])/3;*/
        }
    }
   imshow("Niveau de gris",nivGris);
    // waitKey(0);
   /***************convolution*******************/
    Mat H_gradient;
    Mat kernelH(1, 3, CV_32F);
    kernelH.at<float>(0,0) = 1.0f;
    kernelH.at<float>(0,1) = 0.0f;
    kernelH.at<float>(0,2) = -1.0f;

    cv::Mat kernelH2(3, 1, CV_32F);
    kernelH2.at<float>(0,0) = 1.0f;
    kernelH2.at<float>(1,0) = 0.0f;
    kernelH2.at<float>(2,0) = -1.0f;
    filter2D( nivGris, H_gradient, -1 , kernelH2, Point( -1, -1 ), 0, BORDER_DEFAULT );
    //filter2D( egal, egal, -1 , kernelH, Point( -1, -1 ), 0, BORDER_DEFAULT );
    //imshow("kernel",kernelH2);
   // namedWindow( "filter2D Demo", CV_WINDOW_AUTOSIZE );
    imshow( "filter2D Demo",H_gradient );
  //  imshow( "filter2D Demo",nivGris);
    //waitKey(0);
   
   /************Histogramme et histogramme cumuler*********/
   int his[256];
   histogramme(nivGris,his,256);
   double cumule[256];
   histoCumule(cumule,his,256,nivGris);

    /*******************Egalisation*******************/ 
    Mat egal=Mat::zeros(img.size(),CV_8UC1);
    //cvtColor(img,img,COLOR_BGR2GRAY);
    //equalizeHist(img,egal);
    //equalizeHist(nivGris,egal);
  /*  int max=max2(his,256);
    int min=minImage(his,256,img.rows*img.cols);

    Mat newImage(img.size(),CV_8UC1);
    for(int i=0; i < nivGris.rows; i++){
        for(int j=0; j<nivGris.cols; j++){
            egal.at<uchar>(i,j)=((256-1)/img.rows*img.cols)*cumule[(int) (nivGris.at<uchar>(i,j))];
            newImage.at<uchar>(i,j)=(int)((nivGris.at<uchar>(i,j)- min)*255/(max-min));
        }
    }
*/
    cvtColor(img,img,COLOR_BGR2GRAY);
    equalizeHist(img,egal);

    //imshow("Egalisation",egal);
    //imshow("Normalisation",newImage);
   // waitKey(0);
    

    /************Histogramme****************/ 
    int hist [256];
    
    histogramme(nivGris,hist,256);
  // histogramme(egal,hist,256);
   //histogramme(newImage,hist,256);
   //afficheTab(hist);

    int m = max1(hist,256);
   
    

  

  /***************Seuillage ******************/
    //Seuillage 
    Mat imgSeuil(img.size(),CV_8UC3);
 

    //for(int i =0; i< 256; i=i+20){
      //  cout<<i<<endl;
       
        Mat aidecontour=Mat::zeros(img.size(),CV_8UC1);
       
       //cout<<aide.rows<<endl;
        int i=128;
        //threshold(nivGris,aide,i,255,THRESH_BINARY);
       // threshold(egal,aide,seuil,256,THRESH_BINARY);
       
        threshold(H_gradient,aidecontour,0,256,THRESH_OTSU);
       

       
     
        imshow("aide contour",aidecontour);
        //waitKey(0);
        //string c = "seuillage à "+to_string(i);
       //imshow(c,aide);
        //waitKey(0);
    //}
    /**************les lignes ****************/
   
    Mat lcontour=Mat::zeros(img.size(),CV_8UC1);
    Mat rcontour=Mat::zeros(img.size(),CV_8UC3);
  
   
   

    /*******************Contour *******************/
    for(int i=0; i < img.rows;i++){
        int blanc=0;
        for(int j=0; j < img.cols;j++){
            if(aidecontour.at<uchar>(i,j)==255){
           // if(erode.at<uchar>(i,j)==255){
                blanc++;
            }
        }
        //contour filtre pour img.cols/8
        //egalisation img.cols/3
        if(blanc>img.cols/8){

            for(int j=0; j<img.cols;j++){
                rcontour.at<Vec3b>(i,j)[0]=255;
                rcontour.at<Vec3b>(i,j)[1]=255;
                rcontour.at<Vec3b>(i,j)[2]=255;
            }
        }else{
              for(int j=0; j<img.cols;j++){
                rcontour.at<Vec3b>(i,j)[0]=0;
                rcontour.at<Vec3b>(i,j)[1]=0;
                rcontour.at<Vec3b>(i,j)[2]=0;
            }
        }
        for(int j=0; j < blanc;j++){
            lcontour.at<uchar>(i,j)=255;
        }

    }

    imshow("avoir blanccontour",lcontour);
    imshow("avoir traitecontour",rcontour);
    //waitKey(0);

  
    waitKey(0);

    ifstream image_file("/home/aurelie/Images/projet image/image7.json");
    string ligne;
    vector<string> v1;
   
    vector<string>v3;
    vector<int>v4;
    
    int nombre=8;
    while(getline(image_file,ligne)){
        //cout<<ligne<<endl;
         //cout<<"taille"<<ligne.size()<<endl;
         if(ligne.find("label")!=string::npos){
             v1.push_back(ligne);
           // cout<<ligne.find("label")<<endl;;
         }
         if(ligne.find("points")!=string::npos){
             string l;
             int number=0;
             while(getline(image_file,l)&& number<nombre){
                 v3.push_back(l);
                 number++;
                
             }
             
         }
       
    };

    for(int i=0; i < v1.size(); i++){
        cout<<v1[i]<<endl;
    }

    for(int i=0; i < v3.size(); i++){
        //cout<<v3[i]<<endl;
        try {
         int num2 = stoi(v3[i]);
        // cout << "num2: " << num2 << endl;
         v4.push_back(num2);
     }
        catch (std::exception& e) {
         //cout << e.what() << " caused the error!!!\n";
         }
    }

    cout<<"taille de v4 "<<v4.size()<<endl;

    for(int i =0 ; i < v4.size(); i=i+4){
       /* int x1=v4[i]
        int y1=v4[i+1]
        int x2=v4[i+2]
        int y2=v4[i+3]*/
        line(rcontour,Point(v4[i],v4[i+1]),Point(v4[i+2],v4[i+3]),Scalar(0,255,0),1,LINE_4);
       

    }


    imshow("avoir traitecontour",rcontour);
     waitKey(0);

  //Egaliser ou normalier l'image


    return 0;
}