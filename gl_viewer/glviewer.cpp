﻿/***************************************************************************
 *   Copyright (C) 2021 by Abderrahman Taha                                *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor,Boston, MA 02110-1301 USA             *
 ***************************************************************************/
#include "glviewer.h"
#define BUFFER_OFFSET(i) ((float *)(i))
static int Wresult, Hresult;
static double anglefinal = 0;
static int FistTimecalibrate = -1;
static double hauteur_fenetre, difMaximum, decalage_xo, decalage_yo,
       decalage_zo;
static GLfloat minx = 999999999.0, miny = 999999999.0, minz = 999999999.0,
               maxx = -999999999.0, maxy = -999999999.0, maxz = -999999999.0;
static GLfloat difX, difY, difZ;
static uint CubeStartIndex=0, PlanStartIndex=0, AxesStartIndex=0;
static uint XStartIndex=0, YStartIndex=0, ZStartIndex=0;
static GLfloat AxeArray[3*24]={400.0, 0.0, 0.0,0.0, 0.0, 0.0,
                              0.0, 400.0, 0.0,0.0, 0.0, 0.0,
                              0.0, 0.0, 400.0,0.0, 0.0, 0.0,
                              400.0, 0.0, 0.0,380.0, 10.0, 0.0,
                              380.0, 0.0, 10.0,380.0, -10.0, 0.0,
                               380.0, 0.0, -10.0,380.0, 10.0, 0.0,
                               0.0, 400.0, 0.0,10.0, 380.0, 0.0,
                               0.0, 380.0, 10.0,-10.0, 380.0, 0.0,
                               0.0, 380.0, -10.0,10.0, 380.0, 0.0,
                               0.0, 0.0, 400.0,10.0, 0.0, 380.0,
                               0.0, 10.0, 380.0,-10.0, 0.0, 380.0,
                               0.0, -10.0, 380.0,10.0, 0.0, 380.0
                              };

static GLfloat PlanArray[3*60]=
{
    -150.0, 600.0, -500.0,-150.0, -600.0, -500.0,
    0.0, 600.0, -500.0,0.0, -600.0, -500.0,
    150.0, 600.0, -500.0,150.0, -600.0, -500.0,
    600.0, -150.0, -500.0,-600.0, -150.0, -500.0,
    600.0, 0.0, -500,-600.0, 0.0, -500.0,
    600.0, 150.0, -500, -600.0, 150.0, -500.0,
    -75.0, 600.0, -500,-75.0, -600.0, -500.0,
    -225.0, 600.0, -500,-225.0, -600.0, -500,
    -300.0, 600.0, -500,-300.0, -600.0, -500,
    -375.0, 600.0, -500,-375.0, -600.0, -500,
    -450.0, 600.0, -500,-450.0, -600.0, -500,
    -525.0, 600.0, -500,-525.0, -600.0, -500,
    75.0, 600.0, -500,75.0, -600.0, -500,
    225.0, 600.0, -500,225.0, -600.0, -500,
    300.0, 600.0, -500,300.0, -600.0, -500,
    375.0, 600.0, -500,375.0, -600.0, -500,
    450.0, 600.0, -500,450.0, -600.0, -500,
    525.0, 600.0, -500,525.0, -600.0, -500,
    600.0, -75.0, -500,-600.0, -75.0, -500,
    600.0, -225.0, -500,-600.0, -225.0, -500,
    600.0, -300.0, -500,-600.0, -300.0, -500,
    600.0, -375.0, -500,-600.0, -375.0, -500,
    600.0, -450.0, -500,-600.0, -450.0, -500,
    600.0, -525.0, -500,-600.0, -525.0, -500,
    600.0, 75.0, -500,-600.0, 75.0, -500,
    600.0, 225.0, -500,-600.0, 225.0, -500,
    600.0, 300.0, -500,-600.0, 300.0, -500,
    600.0, 375.0, -500,-600.0, 375.0, -500,
    600.0, 450.0, -500,-600.0, 450.0, -500,
    600.0, 525.0, -500,-600.0, 525.0, -500
};

void OpenGlWidget::CalculateTexturePoints(int type)
{
    double tmp, val[6]={0,0,0,0,0,0};
    uint Jprime;
    if (type == 1)
    {
        LocalScene.componentsinfos.ThereisRGBA[0] = true;
        LocalScene.componentsinfos.NoiseParam[0].NoiseType = 1;
        LocalScene.componentsinfos.NoiseParam[0].RgbtParser =
            IsoObjet->masterthread->RgbtParser;
        LocalScene.componentsinfos.NoiseParam[0].NoiseParser =
            IsoObjet->masterthread->NoiseParser;
        IsoObjet->masterthread->Noise == ""
        ? LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 0
                : LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 1;
    }
    if (type == 0)
    {
        LocalScene.componentsinfos.ThereisRGBA[0] = true;
        LocalScene.componentsinfos.NoiseParam[0].NoiseType = 1;
        LocalScene.componentsinfos.NoiseParam[0].RgbtParser =
            ParObjet->masterthread->RgbtParser;
        LocalScene.componentsinfos.NoiseParam[0].NoiseParser =
            ParObjet->masterthread->NoiseParser;
        ParObjet->masterthread->Noise == ""
        ? LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 0
                : LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 1;
    }
    for (uint i = 0; i < LocalScene.VertxNumber; i++)
    {
        val[0] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 7]) /
                 hauteur_fenetre -
                 decalage_xo;
        val[1] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 8]) /
                 hauteur_fenetre -
                 decalage_yo;
        val[2] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 9]) /
                 hauteur_fenetre -
                 decalage_zo;
        if (type != 1)
        {
            Jprime = (i) % (ParObjet->Vgrid);
            val[3] = double(Jprime) / double(ParObjet->Vgrid);
            val[3] = val[3] * ParObjet->masterthread->dif_u[0] +
                     ParObjet->masterthread->u_inf[0];
            Jprime = (i) / (ParObjet->Ugrid);
            val[4] = double(Jprime) / double(ParObjet->Ugrid);
            val[4] = val[4] * ParObjet->masterthread->dif_v[0] +
                     ParObjet->masterthread->v_inf[0];
        }
        if (LocalScene.componentsinfos.NoiseParam[0].NoiseShape != 0)
        {
            tmp = LocalScene.componentsinfos.NoiseParam[0].NoiseParser->Eval(val);
        }
        else
        {
            tmp = 1.0;
        }
        val[0] *= tmp;
        val[1] *= tmp;
        val[2] *= tmp;
        // val[3] and val[4] uninitialized!
        val[3] *= tmp;
        val[4] *= tmp;
        LocalScene.ArrayNorVer_localPt[i * 10] =
            float(LocalScene.componentsinfos.NoiseParam[0].RgbtParser[0].Eval(val));
        LocalScene.ArrayNorVer_localPt[i * 10 + 1] =
            float(LocalScene.componentsinfos.NoiseParam[0].RgbtParser[1].Eval(val));
        LocalScene.ArrayNorVer_localPt[i * 10 + 2] =
            float(LocalScene.componentsinfos.NoiseParam[0].RgbtParser[2].Eval(val));
        LocalScene.ArrayNorVer_localPt[i * 10 + 3] =
            float(LocalScene.componentsinfos.NoiseParam[0].RgbtParser[3].Eval(val));
    }
}

void OpenGlWidget::CalculatePigmentPoints(int type)
{
    double tmp, ValCol[100], val[4];
    if (type == 1)
    {
        LocalScene.componentsinfos.ThereisRGBA[0] = true;
        LocalScene.componentsinfos.NoiseParam[0].NoiseType = 0; // Pigments
        LocalScene.componentsinfos.NoiseParam[0].VRgbtParser =
            IsoObjet->masterthread->VRgbtParser;
        LocalScene.componentsinfos.NoiseParam[0].GradientParser =
            IsoObjet->masterthread->GradientParser;
        LocalScene.componentsinfos.NoiseParam[0].Nb_vrgbts =
            IsoObjet->masterthread->VRgbtSize;
        LocalScene.componentsinfos.NoiseParam[0].NoiseParser =
            IsoObjet->masterthread->NoiseParser;
        IsoObjet->masterthread->Noise == ""
        ? LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 0
                : LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 1;
    }
    if (type == 0)
    {
        LocalScene.componentsinfos.ThereisRGBA[0] = true;
        LocalScene.componentsinfos.NoiseParam[0].NoiseType = 0; // Pigments
        LocalScene.componentsinfos.NoiseParam[0].VRgbtParser =
            ParObjet->masterthread->VRgbtParser;
        LocalScene.componentsinfos.NoiseParam[0].GradientParser =
            ParObjet->masterthread->GradientParser;
        LocalScene.componentsinfos.NoiseParam[0].Nb_vrgbts =
            ParObjet->masterthread->VRgbtSize;
        LocalScene.componentsinfos.NoiseParam[0].NoiseParser =
            ParObjet->masterthread->NoiseParser;
        ParObjet->masterthread->Noise == ""
        ? LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 0
                : LocalScene.componentsinfos.NoiseParam[0].NoiseShape = 1;
    }
    for (uint i = 0;
            i < LocalScene.componentsinfos.NoiseParam[0].Nb_vrgbts && i < 100; i++)
    {
        ValCol[i] =
            LocalScene.componentsinfos.NoiseParam[0].VRgbtParser[i].Eval(val);
    }
    for (uint i = 0; i < LocalScene.VertxNumber; i++)
    {
        val[0] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 7]) /
                 hauteur_fenetre -
                 decalage_xo;
        val[1] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 8]) /
                 hauteur_fenetre -
                 decalage_yo;
        val[2] = difMaximum *
                 double(LocalScene.ArrayNorVer_localPt[i * 10 + 9]) /
                 hauteur_fenetre -
                 decalage_zo;
        if (LocalScene.componentsinfos.NoiseParam[0].NoiseShape != 0)
            tmp = LocalScene.componentsinfos.NoiseParam[0].NoiseParser->Eval(val);
        else
            tmp = 1.0;
        val[0] *= tmp;
        val[1] *= tmp;
        val[2] *= tmp;
        tmp = LocalScene.componentsinfos.NoiseParam[0].GradientParser->Eval(val);
        int c = int(tmp);
        tmp = std::abs(tmp - double(c));
        for (uint j = 0;
                j < LocalScene.componentsinfos.NoiseParam[0].Nb_vrgbts && j < 100;
                j += 5)
            if (tmp <= ValCol[j])
            {
                LocalScene.ArrayNorVer_localPt[i*10  ] = float(ValCol[j+1]);
                LocalScene.ArrayNorVer_localPt[i*10+1] = float(ValCol[j+2]);
                LocalScene.ArrayNorVer_localPt[i*10+2] = float(ValCol[j+3]);
                LocalScene.ArrayNorVer_localPt[i*10+3] = float(ValCol[j+4]);
                j = 100;
            }
    }
}

int OpenGlWidget::memoryallocation(uint nbthreads,
                                   uint initparGrid, uint initisoGrid,
                                   uint FactX, uint FactY, uint FactZ)
{
    try
    {
        IsoObjet = new Iso3D(nbthreads, initisoGrid, FactX, FactY, FactZ);
        ParObjet = new Par3D(nbthreads, initparGrid);
        return 1;
    }
    catch (std::bad_alloc &)
    {
        message.setText("Not enough memory available to complete this operation");
        message.adjustSize();
        message.exec();
        return -1;
    }
}

void OpenGlWidget::SaveSceneAsObjPoly(int type)
{
    int startpl = 0;
    uint actualpointindice;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save OBJ file"), "",
                       tr("OBJ Files (*.obj)"));

    QFile data(fileName);
    if (data.open(QFile::ReadWrite | QFile::Truncate))
    {
        uint i;
        QTextStream stream(&data);
        stream.setRealNumberNotation(QTextStream::FixedNotation);
        stream.setRealNumberPrecision(3);
        // save vertices:
        if (type == 1)
        {
            for (i = 0; i < LocalScene.VertxNumber; i++)
            {
                (stream) << "v "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 7]) *
                                  (difMaximum*factx/ hauteur_fenetre) -
                                  decalage_xo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 8]) *
                                  (difMaximum*facty / hauteur_fenetre) -
                                  decalage_yo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 9]) *
                                  (difMaximum*factz / hauteur_fenetre) -
                                  decalage_zo)
                         << "  " << LocalScene.ArrayNorVer_localPt[10 * i] << "  "
                         << LocalScene.ArrayNorVer_localPt[10 * i + 1] << "  "
                         << LocalScene.ArrayNorVer_localPt[10 * i + 2] << "\n";
            }
        }
        else
        {
            for (i = 0; i < LocalScene.VertxNumber; i++)
            {
                (stream) << "v "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 7]) *
                                  (difMaximum*factx / hauteur_fenetre) -
                                  decalage_xo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 8]) *
                                  (difMaximum*facty / hauteur_fenetre) -
                                  decalage_yo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 9]) *
                                  (difMaximum*factz / hauteur_fenetre) -
                                  decalage_zo)
                         << "\n";
            }
        }
        // save faces:
        for (i = 0; i < LocalScene.NbPolygnNbVertexPtMin; i++)
        {
            uint polysize = LocalScene.PolyIndices_localPtMin[startpl++];
            (stream) << "f";
            for (uint j = 0; j < polysize; j++)
            {
                actualpointindice = LocalScene.PolyIndices_localPtMin[startpl] + 1;
                (stream) << "  " << actualpointindice;
                startpl++;
            }
            (stream) << "\n";
            i += polysize;
        }
    }
}

void OpenGlWidget::SaveSceneAsObjTrian(int type)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save OBJ file"), "",
                       tr("OBJ Files (*.obj)"));
    QFile data(fileName);
    if (data.open(QFile::ReadWrite | QFile::Truncate))
    {
        uint i;
        QTextStream stream(&data);
        stream.setRealNumberNotation(QTextStream::FixedNotation);
        stream.setRealNumberPrecision(3);
        // save vertices:
        if (type == 1)
        {
            for (i = 0; i < LocalScene.VertxNumber; i++)
            {
                (stream) << "v "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 7]) *
                                  (difMaximum*factx / hauteur_fenetre) -
                                  decalage_xo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 8]) *
                                  (difMaximum*facty / hauteur_fenetre) -
                                  decalage_yo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 9]) *
                                  (difMaximum*factz / hauteur_fenetre) -
                                  decalage_zo)
                         << "  " << LocalScene.ArrayNorVer_localPt[10 * i] << "  "
                         << LocalScene.ArrayNorVer_localPt[10 * i + 1] << "  "
                         << LocalScene.ArrayNorVer_localPt[10 * i + 2] << "\n";
            }
        }
        else
        {
            for (i = 0; i < LocalScene.VertxNumber; i++)
            {
                (stream) << "v "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 7]) *
                                  (difMaximum*factx / hauteur_fenetre) -
                                  decalage_xo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 8]) *
                                  (difMaximum*facty / hauteur_fenetre) -
                                  decalage_yo)
                         << "  "
                         << float(double(LocalScene.ArrayNorVer_localPt[10 * i + 9]) *
                                  (difMaximum*factz / hauteur_fenetre) -
                                  decalage_zo)
                         << "\n";
            }
        }
        for (i = 0; i < LocalScene.PolyNumber; i += 3)
        {
            (stream) << "f "
                     << "  " << LocalScene.PolyIndices_localPt[i] + 1 << "  "
                     << LocalScene.PolyIndices_localPt[i + 1] + 1 << "  "
                     << LocalScene.PolyIndices_localPt[i + 2] + 1 << "\n";
        }
    }
}

OpenGlWidget::~OpenGlWidget()
{
    delete (timer);
    delete ParObjet;
    delete IsoObjet;
}

void OpenGlWidget::deleteVBO() {}
void OpenGlWidget::initbox()
{
    oldminx = 999999999.0;
    oldminy = 999999999.0;
    oldminz = 999999999.0;
    oldmaxx = -999999999.0;
    oldmaxy = -999999999.0;
    oldmaxz = -999999999.0;
}

void OpenGlWidget::PutObjectInsideCube()
{
    minx = 999999999.0;
    miny = 999999999.0;
    minz = 999999999.0;
    maxx = -999999999.0;
    maxy = -999999999.0;
    maxz = -999999999.0;
    if ((LocalScene.morph != 1 ||
            (LocalScene.morph == 1 && FistTimecalibrate == 1)) &&
            LocalScene.slider != 1)
    {
        for (uint i = 0; i < LocalScene.VertxNumber; i++)
        {
            if (minx > LocalScene.ArrayNorVer_localPt[10 * i + 3 + 4])
                minx = LocalScene.ArrayNorVer_localPt[10 * i + 3 + 4];
            if (miny > LocalScene.ArrayNorVer_localPt[10 * i + 4 + 4])
                miny = LocalScene.ArrayNorVer_localPt[10 * i + 4 + 4];
            if (minz > LocalScene.ArrayNorVer_localPt[10 * i + 5 + 4])
                minz = LocalScene.ArrayNorVer_localPt[10 * i + 5 + 4];

            if (maxx < LocalScene.ArrayNorVer_localPt[10 * i + 3 + 4])
                maxx = LocalScene.ArrayNorVer_localPt[10 * i + 3 + 4];
            if (maxy < LocalScene.ArrayNorVer_localPt[10 * i + 4 + 4])
                maxy = LocalScene.ArrayNorVer_localPt[10 * i + 4 + 4];
            if (maxz < LocalScene.ArrayNorVer_localPt[10 * i + 5 + 4])
                maxz = LocalScene.ArrayNorVer_localPt[10 * i + 5 + 4];
        }
        FistTimecalibrate = -1;
        difX = maxx - minx;
        difY = maxy - miny;
        difZ = maxz - minz;
        // Recherche du maximum :
        difMaximum = double(difX);
        if (difY > float(difMaximum))
        {
            difMaximum = double(difY);
        }
        if (difZ > float(difMaximum))
        {
            difMaximum = double(difZ);
        }
        /// On va inclure cet objet dans un cube de langueur maximum
        /// egale a "hauteur_fenetre"
        decalage_xo = -double(minx + maxx) / 2;
        decalage_yo = -double(miny + maxy) / 2;
        decalage_zo = -double(minz + maxz) / 2;
    }
    for (uint i = 0; i < LocalScene.VertxNumber; i++)
    {
        LocalScene.ArrayNorVer_localPt[10 * i + 7] = float(
                    hauteur_fenetre *
                    (double(LocalScene.ArrayNorVer_localPt[10 * i + 7]) + decalage_xo) /
                    difMaximum);
        LocalScene.ArrayNorVer_localPt[10 * i + 8] = float(
                    hauteur_fenetre *
                    (double(LocalScene.ArrayNorVer_localPt[10 * i + 8]) + decalage_yo) /
                    difMaximum);
        LocalScene.ArrayNorVer_localPt[10 * i + 9] = float(
                    hauteur_fenetre *
                    (double(LocalScene.ArrayNorVer_localPt[10 * i + 9]) + decalage_zo) /
                    difMaximum);
    }

    // Cube vertices and indexes
    float longX = 350 * float(difX / float(difMaximum)),
          longY = 350 * (difY / float(difMaximum)),
          longZ = 350 * (difZ / float(difMaximum));
    uint NbVert = LocalScene.VertxNumber;
    CubeStartIndex=NbVert;
    for(uint id=0; id<12; id++)
    {
        LocalScene.ArrayNorVer_localPt[10 * (NbVert+id) + 0] = 0.8f;
        LocalScene.ArrayNorVer_localPt[10 * (NbVert+id) + 1] = 0.8f;
        LocalScene.ArrayNorVer_localPt[10 * (NbVert+id) + 2] = 0.8f;
        LocalScene.ArrayNorVer_localPt[10 * (NbVert+id) + 3] = 1.0f;
    }

    LocalScene.ArrayNorVer_localPt[10 * NbVert + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * NbVert + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * NbVert + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+1) + 7] =  longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+1) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+1) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+2) + 7] =  longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+2) + 8] =  longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+2) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+3) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+3) + 8] =  longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+3) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+4) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+4) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+4) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+5) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+5) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+5) + 9] =  longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+6) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+6) + 8] =  longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+6) + 9] =  longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+7) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+7) + 8] =  longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+7) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+8) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+8) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+8) + 9] = -longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+9) + 7] = -longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+9) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+9) + 9] =  longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+10) + 7] =  longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+10) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+10) + 9] =  longZ;

    LocalScene.ArrayNorVer_localPt[10 * (NbVert+11) + 7] =  longX;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+11) + 8] = -longY;
    LocalScene.ArrayNorVer_localPt[10 * (NbVert+11) + 9] = -longZ;

    // Plan vertices and indexes
    PlanStartIndex=NbVert+12;
    for(uint id=0; id<60; id++)
    {
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+0] = 0.6f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+1] = 0.6f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+2] = 0.6f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+3] = 1.0f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+7] = PlanArray[3*id  ];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+8] = PlanArray[3*id+1];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+id)+9] = PlanArray[3*id+2];
    }

    // Axes vertices and indexes
    AxesStartIndex = NbVert+12+60;
    for(uint id=0; id<6; id++)
    {
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+0] = (id<2)?1.0f:0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+1] = (id>1)&&(id<4)?1.0f:0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+2] = (id>3)?1.0f:0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+3] = 1.0f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+7] = AxeArray[3*id  ];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+8] = AxeArray[3*id+1];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+id)+9] = AxeArray[3*id+2];
    }

    // Head of the X Axe
    XStartIndex = NbVert+12+60+6;
    for(uint id=0; id<6; id++)
    {
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+0] = (id<2)?1.0f:0.3;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+1] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+2] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+3] = 1.0f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+7] = AxeArray[3*(id+6)  ];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+8] = AxeArray[3*(id+6)+1];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+6+id)+9] = AxeArray[3*(id+6)+2];
    }

    // Head of the Y Axe
    YStartIndex = NbVert+12+60+12;
    for(uint id=0; id<6; id++)
    {
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+0] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+1] = (id<2)?1.0f:0.3;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+2] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+3] = 1.0f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+7] = AxeArray[3*(id+12)  ];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+8] = AxeArray[3*(id+12)+1];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+12+id)+9] = AxeArray[3*(id+12)+2];
    }

    // Head of the Z Axe
    ZStartIndex = NbVert+12+60+18;
    for(uint id=0; id<6; id++)
    {
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+0] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+1] = 0.0;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+2] = (id<2)?1.0f:0.3;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+3] = 1.0f;
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+7] = AxeArray[3*(id+18)  ];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+8] = AxeArray[3*(id+18)+1];
        LocalScene.ArrayNorVer_localPt[10*(NbVert+12+60+18+id)+9] = AxeArray[3*(id+18)+2];
    }
}

void OpenGlWidget::mouseReleaseEvent(QMouseEvent *) {}

void OpenGlWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        btgauche = 1;
    else
        btgauche = 0;
    if (e->button() == Qt::RightButton)
        btdroit = 1;
    else
        btdroit = 0;
    if (e->button() == Qt::MidButton)
        btmilieu = 1;
    else
        btmilieu = 0;
    old_y = e->y();
    LocalScene.oldRoty = e->y();
    old_x = e->x();
    LocalScene.oldRotx = e->x();
}

void OpenGlWidget::png()
{
    LocalScene.png_ok *= -1;
}

void OpenGlWidget::jpg()
{
    LocalScene.jpg_ok *= -1;
}

void OpenGlWidget::bmp()
{
    LocalScene.bmp_ok *= -1;
}

void OpenGlWidget::quality(int c)
{
    LocalScene.quality_image = c;
}

void OpenGlWidget::colorstype(int c)
{
    LocalScene.colortype = c;
}

void OpenGlWidget::colorstypeParIso(int c)
{
    LocalScene.colortype = LocalScene.colortypeParam = c;
}

void OpenGlWidget::colorstypeParam(int c)
{
    LocalScene.colortypeParam = c;
}

void OpenGlWidget::redSpec(int cl)
{
    LocalScene.specReflection[0] = float(cl / 100.0);
}

void OpenGlWidget::greenSpec(int cl)
{
    LocalScene.specReflection[1] = float(cl / 100.0);
}

void OpenGlWidget::blueSpec(int cl)
{
    LocalScene.specReflection[2] = float(cl / 100.0);
}

static void drawCube(float x)
{
    float longX = x * float(difX / float(difMaximum)),
          longY = x * (difY / float(difMaximum)),
          longZ = x * (difZ / float(difMaximum));
    double mix = double(minx), max = double(maxx), miy = double(miny),
           may = double(maxy), miz = double(minz), maz = double(maxz);

    glLineWidth(1.0);
    glDrawArrays(GL_LINE_STRIP,CubeStartIndex, 12);

    // X
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(longX, -longY, longZ + 60);
    glCallLists(QString::number(max, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(max, 'g', 3).toLatin1());

    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-longX, -longY, longZ + 60);
    glCallLists(QString::number(mix, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(mix, 'g', 3).toLatin1());
    // Y
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3f(longX + 100, longY, -longZ);
    glCallLists(QString::number(may, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(may, 'g', 3).toLatin1());

    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3f(longX + 100, -longY, -longZ);
    glCallLists(QString::number(miy, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(miy, 'g', 3).toLatin1());
    // Z
    glColor3f(0.4f, 0.4f, 1.0);
    glRasterPos3f(longX + 60, -longY - 60, longZ);
    glCallLists(QString::number(maz, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(maz, 'g', 3).toLatin1());

    glColor3f(0.4f, 0.4f, 1.0);
    glRasterPos3f(longX + 60, -longY - 60, -longZ);
    glCallLists(QString::number(miz, 'g', 3).size(), GL_UNSIGNED_BYTE,
                QString::number(miz, 'g', 3).toLatin1());
}

static void DrawPariso(ObjectProperties *scene, uint ParisoTypeIndex)
{
    uint idx = 0;
    for (uint i = 0; i < ParisoTypeIndex; i++)
        idx += scene->componentsinfos.NbComponentsType[i];
    int start_triangle = scene->componentsinfos.ParisoTriangle[2 * idx];
    float frontcl[4], backcl[4];
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_POLYGON_OFFSET_FILL);

    if (scene->componentsinfos.ThereisRGBA[ParisoTypeIndex])
        glEnable(GL_COLOR_MATERIAL);

    glPolygonOffset(scene->polyfactor, scene->polyunits);

    if (!scene->componentsinfos.ThereisRGBA[ParisoTypeIndex])
    {
        for (uint j = 0; j < 4; j++)
        {
            frontcl[j] = scene->frontcols[j];
            backcl[j] = scene->backcols[j];
        }
        glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, backcl);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontcl);
    }
    if (scene->componentsinfos.ThereisCND[ParisoTypeIndex])
    {
        size_t Offset0 = start_triangle*sizeof( GL_FLOAT);
        glDrawElements(
            GL_TRIANGLES,
            int(3 * (scene->componentsinfos.NbTrianglesNoCND[ParisoTypeIndex])),
            GL_UNSIGNED_INT, (void *)(Offset0));

        if (scene->cndoptions[0])
        {
            size_t Offset1 = (3 * scene->componentsinfos.NbTrianglesNoCND[ParisoTypeIndex] +start_triangle)*sizeof( GL_FLOAT);
            glDrawElements(
                GL_TRIANGLES,
                int(3 * (scene->componentsinfos.NbTrianglesVerifyCND[ParisoTypeIndex])),
                GL_UNSIGNED_INT, (void *)(Offset1));
        }

        if (scene->cndoptions[1])
        {
            size_t Offset2 = (3 * scene->componentsinfos.NbTrianglesNoCND[ParisoTypeIndex] +
                              3 * scene->componentsinfos.NbTrianglesVerifyCND[ParisoTypeIndex] +
                              start_triangle)*sizeof( GL_FLOAT);
            glDrawElements(
                GL_TRIANGLES,
                int(3 * (scene->componentsinfos.NbTrianglesNotVerifyCND[ParisoTypeIndex])),
                GL_UNSIGNED_INT,
                (void *)(Offset2));
        }
    }
    else
    {
        for (uint i = 0; i < scene->componentsinfos.NbComponentsType[ParisoTypeIndex]; i++)
        {
            uint cmpIndex = (ParisoTypeIndex==1) ? (scene->componentsinfos.NbComponentsType[0]+i):i;
            if (scene->componentsinfos.ShowParIsoCmp[cmpIndex])
            {
                if (!scene->componentsinfos.ThereisRGBA[ParisoTypeIndex])
                {
                    for (uint j = 0; j < 4; j++)
                    {
                        frontcl[j] = scene->frontcols[4 * (i % 10) + j];
                        backcl[j] = scene->backcols[4 * (i % 10) + j];
                    }
                    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, backcl);
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontcl);
                }
                {
                    size_t Offset = scene->componentsinfos.ParisoTriangle[2*(i+idx)]*sizeof( GL_FLOAT);
                    glDrawElements(
                        GL_TRIANGLES,
                        int(3 * scene->componentsinfos.ParisoTriangle[2 * (i + idx) + 1]),GL_UNSIGNED_INT,(void *)(Offset));
                }
            }
        }
    }
    if (scene->componentsinfos.ThereisRGBA[ParisoTypeIndex])
        glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}
void OpenGlWidget::normOk()
{
    LocalScene.norm *= -1;
    update();
}
void OpenGlWidget::infosOk()
{
    LocalScene.infos *= -1;
    update();
}
void OpenGlWidget::smoothline()
{
    LocalScene.smoothline *= -1;
    /// For drawing Lines :
    if (LocalScene.smoothline == 1)
    {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    }
    else
    {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    update();
}
void OpenGlWidget::boundingboxOk()
{
    LocalScene.boundingbox *= -1;
}

static GLuint fontOffset = 0;
static void makeRasterFont()
{
    GLuint i;
    if (fontOffset < 128)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        fontOffset = glGenLists(128);
        for (i = 31;i  < 128; i++)
        {
            glNewList(i + fontOffset, GL_COMPILE);
            glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, rasters[i-31]);
            glEndList();
        }
    }
}

void OpenGlWidget::run() {}

bool OpenGlWidget::timeractif()
{
    return (LocalScene.anim == 1);
}

void OpenGlWidget::stoptimer()
{
    timer->stop();
    update();
}

void OpenGlWidget::starttimer()
{
    timer->start(latence);
}

void OpenGlWidget::restarttimer(int newlatence)
{
    latence = newlatence;
    if (LocalScene.anim == 1)
    {
        timer->stop();
        timer->start(latence);
    }
}

void OpenGlWidget::startRendering()
{
    glt.start();
}

void OpenGlWidget::stopRendering()
{
    glt.stop();
    glt.wait();
}

void OpenGlWidget::resizeEvent(QResizeEvent *evt)
{
    glPushMatrix();
    glt.resizeViewport(evt->size());
    glPopMatrix();
}

void OpenGlWidget::closeEvent(QCloseEvent *evt)
{
    stopRendering();
    QGLWidget::closeEvent(evt);
}

void OpenGlWidget::anim()
{
    LocalScene.anim *= -1;
    if (LocalScene.anim == 1)
        timer->start(latence);
    else if (LocalScene.morph == -1)
        stoptimer();
}

void OpenGlWidget::morph()
{
    LocalScene.morph *= -1;
    FistTimecalibrate *= -1;

    if (LocalScene.typedrawing == 1)
    {
        // Isosurfaces:
        IsoObjet->masterthread->activeMorph = LocalScene.morph;
        for (uint nbthreads = 0; nbthreads < IsoObjet->WorkerThreadsNumber - 1;
                nbthreads++)
            IsoObjet->workerthreads[nbthreads].activeMorph = LocalScene.morph;
        IsoObjet->IsoMorph();
    }
    else if (LocalScene.typedrawing == -1)
    {
        // Parametric surfaces:
        ParObjet->masterthread->activeMorph = LocalScene.morph;
        for (uint nbthreads = 0; nbthreads < ParObjet->WorkerThreadsNumber - 1;
                nbthreads++)
            ParObjet->workerthreads[nbthreads].activeMorph = LocalScene.morph;
        ParObjet->ParMorph();
    }
    else   // Pariso objects
    {
        // Parametric surfaces:
        ParObjet->masterthread->activeMorph = LocalScene.morph;
        for (uint nbthreads = 0; nbthreads < ParObjet->WorkerThreadsNumber - 1;
                nbthreads++)
            ParObjet->workerthreads[nbthreads].activeMorph = LocalScene.morph;
        ParObjet->ParMorph();
        // Isosurfaces:
        IsoObjet->masterthread->activeMorph = LocalScene.morph;
        for (uint nbthreads = 0; nbthreads < IsoObjet->WorkerThreadsNumber - 1;
                nbthreads++)
            IsoObjet->workerthreads[nbthreads].activeMorph = LocalScene.morph;
        IsoObjet->IsoMorph();
    }
    if (LocalScene.morph == 1)
        timer->start(latence);
    else if (LocalScene.anim == -1)
        stoptimer();
}

void OpenGlWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    switch (key)
    {
    case Qt::Key_A:
        glt.anim();
        break;
    case Qt::Key_P:
        glt.morph();
        initbox();
        break;
    }
    glt.update();
}

void OpenGlWidget::resizeGL(int newwidth, int newheight)
{
    Wresult = newwidth;
    Hresult = newheight;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, newwidth, newheight);
    glFrustum(-newwidth / 6.0, newwidth / 6.0, -newheight / 6.0, newheight / 6.0, 250.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0, -1000.0);
}

OpenGlWidget::OpenGlWidget(QWidget *parent) : QGLWidget(parent), glt(this)
{
    setAutoBufferSwap(true);
    static int NBGlWindow = 0;
    PerlinNoise = new ImprovedNoise(4., 4., 4.);

    latence = 10;
    val1 = val2 = val3 = 0.0;
    Vgrid = Ugrid = 50;
    CutV = CutU = 0;
    Xgrid = Ygrid = Zgrid = 40;
    CutX = CutY = CutZ = 0;
    IDGlWindow = NBGlWindow;
    LocalScene.VertxNumber = 0;
    FramesDir = "/home";
    hauteur_fenetre = 700;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    NBGlWindow++;
}

void OpenGlWidget::infosok()
{
    LocalScene.infos *= -1;
}

void OpenGlWidget::FillOk()
{
    LocalScene.fill *= -1;
    update();
}

void OpenGlWidget::PrintInfos()
{
    QString nbl = "";
    (LocalScene.typedrawing == 1)
    ? nbl = QString::number(Xgrid - CutX) + "x" +
            QString::number(Ygrid - CutY) + "x" +
            QString::number(Zgrid - CutZ)

    : nbl = QString::number(Ugrid - CutU) + "x" +
            QString::number(Vgrid - CutV) + " = " +
            QString::number((Ugrid - CutU) * (Vgrid - CutV));
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, Wresult, Hresult, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (LocalScene.infosdetails[0] == 1)
    {
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2i(10, 20);
        glCallLists(strlen("Grid = "), GL_UNSIGNED_BYTE, (GLubyte *)"Grid = ");
        glColor3f(1., 0.0, 0.);
        glRasterPos2i(80, 20);
        glCallLists(nbl.size(), GL_UNSIGNED_BYTE, nbl.toLatin1());
    }
    if (LocalScene.infosdetails[1] == 1)
    {
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2i(10, 40);
        glCallLists(strlen("Poly = "), GL_UNSIGNED_BYTE, (GLubyte *)"Poly = ");
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2i(80, 40);
        glCallLists(QString::number(LocalScene.PolyNumber / 3).size(),
                    GL_UNSIGNED_BYTE,
                    QString::number(LocalScene.PolyNumber / 3).toLatin1());
    }
    if (LocalScene.infosdetails[2] == 1)
    {
        if (LocalScene.typedrawing == 1)
        {
            glColor3f(0.0, 1.0, 0.0);
            glRasterPos2i(10, 60);
            glCallLists(strlen("Vertx= "), GL_UNSIGNED_BYTE, (GLubyte *)"Vertx= ");
            glColor3f(1.0, 0.0, 0.0);
            glRasterPos2i(80, 60);
            glCallLists(QString::number(LocalScene.VertxNumber).size(),
                        GL_UNSIGNED_BYTE,
                        QString::number(LocalScene.VertxNumber).toLatin1());
        }
    }
    if (LocalScene.anim == 1)
    {
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2i(10, 80);
        glCallLists(strlen("Anim ="), GL_UNSIGNED_BYTE, (GLubyte *)"Anim =");
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2i(80, 80);
        glCallLists(strlen("On"), GL_UNSIGNED_BYTE, (GLubyte *)"On");
    }
    if (LocalScene.morph == 1)
    {
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2i(10, 100);
        glCallLists(strlen("Morph="), GL_UNSIGNED_BYTE, (GLubyte *)"Morph=");
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2i(80, 100);
        glCallLists(strlen("On"), GL_UNSIGNED_BYTE, (GLubyte *)("On"));
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

static void DrawAxe()
{

    glLineWidth(1.0);

    // Draw the three axes (lines without head)
    glDrawArrays(GL_LINES,AxesStartIndex,6);

    // Head of the X Axe:
    glDrawArrays(GL_TRIANGLE_FAN,XStartIndex,6);

    // Head of the Y Axe:
    glDrawArrays(GL_TRIANGLE_FAN,YStartIndex,6);

    // Head of the Z Axe:
    glDrawArrays(GL_TRIANGLE_FAN,ZStartIndex,6);

    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3i(410, 10, 10);
    glCallLists(strlen("X"), GL_UNSIGNED_BYTE, "X");

    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3i(10, 410, 10);
    glCallLists(strlen("Y"), GL_UNSIGNED_BYTE, "Y");

    glColor3f(0.0, 0.0, 1.0);
    glRasterPos3i(10, 10, 410);
    glCallLists(strlen("Z"), GL_UNSIGNED_BYTE, "Z");
}

static void DrawNormals(ObjectProperties *scene)
{
    uint j = 0;
    glColor4f(0.8f, 0., 0.7f, 1.0);
    for (uint i = 0; i < scene->PolyNumber; i += 4)
    {
        j = 10 * scene->PolyIndices_localPt[i];
        glBegin(GL_LINES);
        glVertex3f(scene->ArrayNorVer_localPt[j + 7],
                   scene->ArrayNorVer_localPt[j + 8],
                   scene->ArrayNorVer_localPt[j + 9]);
        glVertex3f(scene->ArrayNorVer_localPt[j + 7] +
                   40 * scene->ArrayNorVer_localPt[j + 4],
                   scene->ArrayNorVer_localPt[j + 8] +
                   40 * scene->ArrayNorVer_localPt[j + 5],
                   scene->ArrayNorVer_localPt[j + 9] +
                   40 * scene->ArrayNorVer_localPt[j + 6]);
        glEnd();
    }
}



void OpenGlWidget::Winitialize_GL()
{
    /*
    static int count = 0;
    if (count < 2)
    {
        /// For drawing Filled Polygones :
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glEnable(GL_NORMALIZE);
        glFrontFace(GL_CCW);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LocalScene.frontcol);
        glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, LocalScene.backcol);

        glMaterialfv(GL_FRONT, GL_SPECULAR, LocalScene.specReflection);
        glMaterialfv(GL_BACK, GL_SPECULAR, LocalScene.specReflection);

        glMateriali(GL_FRONT, GL_SHININESS, LocalScene.shininess);
        glMateriali(GL_BACK, GL_SHININESS, LocalScene.shininess);
        glEnable(GL_DEPTH_TEST);

        glClearColor(LocalScene.groundcol[0], LocalScene.groundcol[1],
                     LocalScene.groundcol[2], LocalScene.groundcol[3]);
        count += 1;
    }
    */
    if (LocalScene.componentsinfos.updateviewer)
        PutObjectInsideCube();
}

void OpenGlWidget::UpdateGL()
{
    if (LocalScene.updategl)
    {
        Winitialize_GL();
        update();
    }
    else
        LocalScene.updategl = true;
}

static void DrawParisoCND(ObjectProperties *scene, uint compindex)
{
    uint idx = 0;
    for (uint i = 0; i < compindex; i++)
        idx += scene->componentsinfos.NbComponentsType[i];
    int start_triangle = scene->componentsinfos.ParisoTriangle[2 * idx];
    if (scene->cndoptions[3])
    {
        size_t Offset0 = (3 * scene->componentsinfos.NbTrianglesNoCND[compindex] + start_triangle)*sizeof( GL_FLOAT);
        glLineWidth(0.3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(
            GL_TRIANGLES,
            int(3 * scene->componentsinfos.NbTrianglesVerifyCND[compindex]),
            GL_UNSIGNED_INT, (void *)Offset0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (scene->cndoptions[4])
    {
        size_t Offset1 = (3 * scene->componentsinfos
                          .NbTrianglesNoCND[compindex] + 3 * scene->componentsinfos
                                                 .NbTrianglesVerifyCND[compindex] +
                                                 start_triangle)*sizeof(GL_FLOAT);
        glLineWidth(0.3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(
            GL_TRIANGLES,
            int(3 * scene->componentsinfos.NbTrianglesNotVerifyCND[compindex]),
            GL_UNSIGNED_INT,(void *)Offset1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (scene->cndoptions[2])
    {
        size_t Offset2 = (3*scene->componentsinfos.NbTrianglesNoCND[compindex]+3*(scene->componentsinfos.NbTrianglesVerifyCND[compindex] +
                          scene->componentsinfos.NbTrianglesNotVerifyCND[compindex])+start_triangle)*sizeof( GL_FLOAT);
        glLineWidth(4.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(
            GL_TRIANGLES,
            int(3 * scene->componentsinfos.NbTrianglesBorderCND[compindex]),
            GL_UNSIGNED_INT,(void *)Offset2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

static void DrawMeshIso(ObjectProperties *scene)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, int(scene->PolyNumber), GL_UNSIGNED_INT, (void *)0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void DrawMinimalTopology(ObjectProperties *scene)
{
    //glColor4f(scene->gridcol[0], scene->gridcol[1], scene->gridcol[2], scene->gridcol[3]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    uint st = scene->PolyNumber;
    uint polysize=0;
    for (uint i = 0; i < scene->NbPolygnNbVertexPtMin; i++)
    {
        polysize = scene->PolyIndices_localPtMin[i];
        size_t Offset = st*sizeof( GL_FLOAT);
        glDrawElements(
            GL_LINE_LOOP,
            polysize,
            GL_UNSIGNED_INT,
            (void *)(Offset));
        st+=(polysize);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void plan()
{
    glLineWidth(0.3);
    glDrawArrays(GL_LINES,PlanStartIndex,60);
}



GLuint vao, vbo[2]; /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */
/* These pointers will receive the contents of our shader source code files */
    GLchar *vertexsource, *fragmentsource;
static void InitialOperations(ObjectProperties *scene)
{
    static int staticaction = 0;
    if (staticaction < 1)
    {/*
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

        // enable vertex arrays
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        size_t cOffset = 0;
        size_t nOffset = cOffset + 4*sizeof( GL_FLOAT);
        size_t vOffset = nOffset + 3*sizeof (GL_FLOAT);

        // specify vertex arrays with their offsets
        glColorPointer(4, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)cOffset);
        glNormalPointer(GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)nOffset);
        glVertexPointer(3, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)vOffset);
*/

        /*
        // activate vertex attribs
        glEnableVertexAttribArray(GL_COLOR_ARRAY);
        glEnableVertexAttribArray(GL_NORMAL_ARRAY);
        glEnableVertexAttribArray(GL_VERTEX_ARRAY);
        // set attrib offsets using glVertexAttribPointer()

        glVertexAttribPointer(GL_COLOR_ARRAY,  4, GL_FLOAT, GL_FALSE, 10*sizeof( GL_FLOAT), scene->ArrayNorVer_localPt);
        glVertexAttribPointer(GL_NORMAL_ARRAY, 3, GL_FLOAT, GL_FALSE, 10*sizeof( GL_FLOAT), &(scene->ArrayNorVer_localPt[4]));
        glVertexAttribPointer(GL_VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, 10*sizeof( GL_FLOAT), &(scene->ArrayNorVer_localPt[7]));
        */



        /// For drawing Filled Polygones :
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glEnable(GL_NORMALIZE);
        glFrontFace(GL_CCW);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, scene->frontcol);
        glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, scene->backcol);

        glMaterialfv(GL_FRONT, GL_SPECULAR, scene->specReflection);
        glMaterialfv(GL_BACK, GL_SPECULAR, scene->specReflection);

        glMateriali(GL_FRONT, GL_SHININESS, scene->shininess);
        glMateriali(GL_BACK, GL_SHININESS, scene->shininess);
        glEnable(GL_DEPTH_TEST);

        glClearColor(scene->groundcol[0], scene->groundcol[1],
                     scene->groundcol[2], scene->groundcol[3]);

        glRotatef(270, 1.0, 0.0, 0.0);
        glRotatef(225, 0.0, 0.0, 1.0);
        glRotatef(-29, 1.0, -1.0, 0.0);
        staticaction += 1;

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

        // Gl listes generation (Fonts):
        makeRasterFont();
    }
}


static void CopyData(ObjectProperties *scene)
{
    static int firstaction=0;
    static uint previousVertxNumber=0;
    static uint previousPolyNumberNbPolygnNbVertexPtMin=0;
    uint MiniPolySize=0;
    if(firstaction==0)
    {
        //glDeleteBuffers(2, vbo);
        vbo[0]=vbo[1]=0;
        glGenBuffers(2, vbo);
        /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10*(scene->VertxNumber+(12+60+24)),scene->ArrayNorVer_localPt, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

        /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint)*(scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize), scene->PolyIndices_localPt, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

        // enable vertex arrays
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        size_t cOffset = 0;
        size_t nOffset = cOffset + 4*sizeof( GL_FLOAT);
        size_t vOffset = nOffset + 3*sizeof (GL_FLOAT);
        // specify vertex arrays with their offsets
        glColorPointer(4, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)cOffset);
        glNormalPointer(GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)nOffset);
        glVertexPointer(3, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)vOffset);
        previousVertxNumber = scene->VertxNumber;
        previousPolyNumberNbPolygnNbVertexPtMin = (scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize);
        firstaction++;
    }
    else{

        if(scene->VertxNumber>previousVertxNumber)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10*(scene->VertxNumber+(12+60+24)), scene->ArrayNorVer_localPt, GL_STATIC_DRAW);
            previousVertxNumber = scene->VertxNumber;
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*10*(scene->VertxNumber+(12+60+24)), scene->ArrayNorVer_localPt);
            previousVertxNumber = scene->VertxNumber;
        }

        if((scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize)>previousPolyNumberNbPolygnNbVertexPtMin)
        {
           glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint)*(scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize), scene->PolyIndices_localPt, GL_STATIC_DRAW);
           previousPolyNumberNbPolygnNbVertexPtMin =  (scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize);

        }
        else
        {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, sizeof(uint)*(scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize), scene->PolyIndices_localPt);
            previousPolyNumberNbPolygnNbVertexPtMin =  (scene->PolyNumber + scene->NbPolygnNbVertexPtMinSize);
        }

        // enable vertex arrays
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        size_t cOffset = 0;
        size_t nOffset = cOffset + 4*sizeof( GL_FLOAT);
        size_t vOffset = nOffset + 3*sizeof (GL_FLOAT);
        // specify vertex arrays with their offsets
        glColorPointer(4, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)cOffset);
        glNormalPointer(GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)nOffset);
        glVertexPointer(3, GL_FLOAT, 10*sizeof( GL_FLOAT), (void*)vOffset);


    }
}

static void draw(ObjectProperties *scene)
{
    InitialOperations(scene);
    if (scene->componentsinfos.Interleave)
    {
        //glInterleavedArrays(GL_C4F_N3F_V3F, 0, scene->ArrayNorVer_localPt);
        CopyData(scene);
        scene->componentsinfos.Interleave = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Blend Effect activation:
    if (scene->transparency == 1)
        glDepthMask(GL_FALSE);

    // Ratation (Animation):
    if (scene->anim == 1 && scene->animxyz == 1)
    {
        glRotatef(scene->RotStrength, float(scene->axe_x), float(scene->axe_y),
                  float(scene->axe_z));
    }

    // Plan:
    if (scene->infos == 1)
    {
        plan();
    }

    // Axe :
    if (scene->infos == 1)
        DrawAxe();

    glPushMatrix();

    if (scene->anim == 1 && scene->animx == 1)
    {
        scene->animxValue += scene->animxValueStep;
        glRotatef(scene->animxValue, 1.0, 0, 0);
    }

    if (scene->anim == 1 && scene->animy == 1)
    {
        scene->animyValue += scene->animyValueStep;
        glRotatef(scene->animyValue, 0, 1.0, 0);
    }

    if (scene->anim == 1 && scene->animz == 1)
    {
        scene->animzValue += scene->animzValueStep;
        glRotatef(scene->animzValue, 0, 0, 1.0);
    }

    if (scene->fill == 1 && scene->componentsinfos.updateviewer)
        for (uint i = 0; i < scene->componentsinfos.NbComponentsType.size(); i++)
            DrawPariso(scene, i);

    // Draw Mesh Object:
    if (scene->triangles == 1 && scene->componentsinfos.updateviewer)
        DrawMeshIso(scene);

    // Bounding Box:
    if (scene->boundingbox == 1)
        drawCube(350);

    // Draw Minimal topology for isosurfaces:
    if (scene->mesh == 1 && scene->componentsinfos.updateviewer)
        DrawMinimalTopology(scene);

    if (scene->activarecnd && scene->componentsinfos.updateviewer)
        for (uint i = 0; i < scene->componentsinfos.NbComponentsType.size(); i++)
            if (scene->componentsinfos.ThereisCND[i])
                DrawParisoCND(scene, i);

    // Draw Normales:
    if (scene->norm == 1 && scene->componentsinfos.updateviewer)
        DrawNormals(scene);

    glPopMatrix();

    if (scene->transparency == 1)
        glDepthMask(GL_TRUE);

    // Draw the scene:
    glFlush();
}

void OpenGlWidget::paintGL()
{
    if (LocalScene.morph == 1)
    {
        if (LocalScene.typedrawing == -1)
        {
            ParObjet->ParamBuild(
                &(LocalScene.ArrayNorVer_localPt), &(LocalScene.PolyIndices_localPt),
                &LocalScene.PolyNumber, &LocalScene.VertxNumber,
                &(LocalScene.componentsinfos), &(LocalScene.PolyIndices_localPtMin),
                &(LocalScene.NbPolygnNbVertexPtMin));
        }
        else if (LocalScene.typedrawing == 1)
        {
            IsoObjet->IsoBuild(
                &(LocalScene.ArrayNorVer_localPt), &(LocalScene.PolyIndices_localPt),
                &(LocalScene.PolyNumber), &(LocalScene.VertxNumber),
                &(LocalScene.PolyIndices_localPtMin),
                &(LocalScene.NbPolygnNbVertexPtMin),
                &(LocalScene.NbPolygnNbVertexPtMinSize),
                &(LocalScene.componentsinfos));
        }
        else
        {

            ParObjet->ParamBuild(
                &(LocalScene.ArrayNorVer_localPt), &(LocalScene.PolyIndices_localPt),
                &LocalScene.PolyNumber, &LocalScene.VertxNumber,
                &(LocalScene.componentsinfos), &(LocalScene.PolyIndices_localPtMin),
                &(LocalScene.NbPolygnNbVertexPtMin));
            IsoObjet->IsoBuild(
                &(LocalScene.ArrayNorVer_localPt), &(LocalScene.PolyIndices_localPt),
                &(LocalScene.PolyNumber), &(LocalScene.VertxNumber),
                &(LocalScene.PolyIndices_localPtMin),
                &(LocalScene.NbPolygnNbVertexPtMin),
                &(LocalScene.NbPolygnNbVertexPtMinSize),
                &(LocalScene.componentsinfos));
        }
        Winitialize_GL();
    }

    draw(&LocalScene);

    if (LocalScene.infos == 1)
        PrintInfos();
    if (LocalScene.morph == 1 && LocalScene.frame == 1)
        FramesSave();
}

void OpenGlWidget::timerEvent(QTimerEvent *)
{
    update();
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent *e)
{
    static double m[16];
    LocalScene.RotStrength =
        sqrt((LocalScene.oldRotx - e->x()) * (LocalScene.oldRotx - e->x()) +
             (LocalScene.oldRoty - e->y()) * (LocalScene.oldRoty - e->y())) /
        2;
    LocalScene.oldRoty = e->y();
    LocalScene.oldRotx = e->x();

    // Scale function :
    if (btdroit == 1)
    {
        if (old_y - e->y() > 0)
            LocalScene.ScalCoeff = 1.02;
        else if (LocalScene.ScalCoeff > 0.1)
            LocalScene.ScalCoeff = 0.98;
        glScalef(GLfloat(LocalScene.ScalCoeff), GLfloat(LocalScene.ScalCoeff),
                 GLfloat(LocalScene.ScalCoeff));
        LocalScene.view_rotx = LocalScene.view_roty = 0.0;
    }
    // Rotational function :
    if (btgauche == 1)
    {
        LocalScene.view_roty = -(old_y - e->y());
        LocalScene.view_rotx = -(old_x - e->x());
        LocalScene.ScalCoeff = 1.0;

        glGetIntegerv(GL_VIEWPORT, LocalScene.viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, LocalScene.matrix);
        memcpy(m, LocalScene.matrix, 16 * sizeof(GLdouble));

        ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
        GLdouble det;
        GLdouble d12, d13, d23, d24, d34, d41;
        GLdouble tmp[16]; /* Allow out == in. */

        /* Inverse = adjoint / det. (See linear algebra texts.)*/
        /* pre-compute 2x2 dets for last two rows when computing */
        /* cofactors of first two rows. */
        d12 = (m31 * m42 - m41 * m32);
        d13 = (m31 * m43 - m41 * m33);
        d23 = (m32 * m43 - m42 * m33);
        d24 = (m32 * m44 - m42 * m34);
        d34 = (m33 * m44 - m43 * m34);
        d41 = (m34 * m41 - m44 * m31);

        tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
        tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
        tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
        tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

        /* Compute determinant as early as possible using these cofactors. */
        det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

        /* Run singularity test. */
        if (det != 0.0)
        {
            GLdouble invDet = 1.0 / det;
            /* Compute rest of inverse. */
            tmp[0] *= invDet;
            tmp[1] *= invDet;
            tmp[2] *= invDet;
            tmp[3] *= invDet;

            tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
            tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
            tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
            tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

            d12 = m11 * m22 - m21 * m12;
            d13 = m11 * m23 - m21 * m13;
            d23 = m12 * m23 - m22 * m13;
            d24 = m12 * m24 - m22 * m14;
            d34 = m13 * m24 - m23 * m14;
            d41 = m14 * m21 - m24 * m11;

            tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
            tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
            tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
            tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
            tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
            tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
            tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
            tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;
            memcpy(LocalScene.matrixInverse, tmp, 16 * sizeof(GLdouble));
        }
        double ax, ay;
        ax = LocalScene.view_roty;
        ay = LocalScene.view_rotx;
        anglefinal += (angle = sqrt(ax * ax + ay * ay) /
                               double(LocalScene.viewport[2] + 1) * 360.0);
        LocalScene.axe_x = Axe_x =
                               LocalScene.matrixInverse[0] * ax + LocalScene.matrixInverse[4] * ay;
        LocalScene.axe_y = Axe_y =
                               LocalScene.matrixInverse[1] * ax + LocalScene.matrixInverse[5] * ay;
        LocalScene.axe_z = Axe_z =
                               LocalScene.matrixInverse[2] * ax + LocalScene.matrixInverse[6] * ay;
        glRotatef(GLfloat(angle), GLfloat(Axe_x), GLfloat(Axe_y), GLfloat(Axe_z));
    }
    old_y = e->y();
    old_x = e->x();
    update();
}

void OpenGlWidget::screenshot()
{
    QImage image = grabFrameBuffer();
    if (LocalScene.png_ok == 1)
        image.save("GLscreenshot.png", "PNG", LocalScene.quality_image);
    if (LocalScene.bmp_ok == 1)
        image.save("GLscreenshot.bmp", "BMP", LocalScene.quality_image);
}

void OpenGlWidget::FramesShot()
{
    LocalScene.frame *= -1;
    if (LocalScene.frame == 1)
    {
        FramesDir = QFileDialog::getExistingDirectory(
                        this, tr("Choose Or Create Directory"), FramesDir,
                        QFileDialog::DontResolveSymlinks);
        if (FramesDir != "" && !FramesDir.endsWith("/"))
            FramesDir += "/";
    }
}

void OpenGlWidget::FramesSave()
{
    static int Index = 0;
    if (LocalScene.frame == 1)
    {
        QImage image = grabFrameBuffer();
        QString FileName =
            FramesDir + QString("%1").arg(Index, 5, 10, QChar('0')) + ".png";
        Index += 1;
        image.save(FileName, "PNG", 1);
    }
}

QImage OpenGlWidget::Copyscreenshot()
{
    return (grabFrameBuffer());
}

void OpenGlWidget::transparency(int cl, int currentposition)
{
    switch (LocalScene.colortype)
    {
    case 1:
        LocalScene.frontcols[(currentposition % 10) * 4 + 3] = (cl / 255.0f);
        break;
    case 0:
        LocalScene.backcols[(currentposition % 10) * 4 + 3] = (cl / 255.0f);
        break;
    case 2:
        LocalScene.gridcol[3] = (cl / 255.0f);
        break;
    case 3:
        LocalScene.groundcol[3] = (cl / 255.0f);
        glClearColor(LocalScene.groundcol[0], LocalScene.groundcol[1],
                     LocalScene.groundcol[2], LocalScene.groundcol[3]);
        break;
    }
    update();
}

void OpenGlWidget::red(int cl, int currentposition)
{
    switch (LocalScene.colortype)
    {
    case 1:
        LocalScene.frontcols[(currentposition % 10) * 4 + 0] = (cl / 255.0f);
        break;
    case 0:
        LocalScene.backcols[(currentposition % 10) * 4 + 0] = (cl / 255.0f);
        break;
    case 2:
        LocalScene.gridcol[0] = (cl / 255.0f);
        break;
    case 3:
        LocalScene.groundcol[0] = (cl / 255.0f);
        glClearColor(LocalScene.groundcol[0], LocalScene.groundcol[1],
                     LocalScene.groundcol[2], LocalScene.groundcol[3]);
        break;
    }
    update();
}

void OpenGlWidget::green(int cl, int currentposition)
{
    switch (LocalScene.colortype)
    {
    case 1:
        LocalScene.frontcols[(currentposition % 10) * 4 + 1] = (cl / 255.0f);
        break;
    case 0:
        LocalScene.backcols[(currentposition % 10) * 4 + 1] = (cl / 255.0f);
        break;
    case 2:
        LocalScene.gridcol[1] = (cl / 255.0f);
        break;
    case 3:
        LocalScene.groundcol[1] = (cl / 255.0f);
        glClearColor(LocalScene.groundcol[0], LocalScene.groundcol[1],
                     LocalScene.groundcol[2], LocalScene.groundcol[3]);
        break;
    }
    update();
}

void OpenGlWidget::blue(int cl, int currentposition)
{
    switch (LocalScene.colortype)
    {
    case 1:
        LocalScene.frontcols[(currentposition % 10) * 4 + 2] = (cl / 255.0f);
        break;
    case 0:
        LocalScene.backcols[(currentposition % 10) * 4 + 2] = (cl / 255.0f);
        break;
    case 2:
        LocalScene.gridcol[2] = (cl / 255.0f);
        break;
    case 3:
        LocalScene.groundcol[2] = (cl / 255.0f);
        glClearColor(LocalScene.groundcol[0], LocalScene.groundcol[1],
                     LocalScene.groundcol[2], LocalScene.groundcol[3]);
        break;
    }
    update();
}

void OpenGlWidget::transparence(bool trs)
{
    LocalScene.transparency *= -1;
    if (trs)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        glDisable(GL_BLEND);

    update();
}

void OpenGlWidget::transSpec(int cl)
{
    LocalScene.specReflection[3] = (cl / 100.0f);
    InitSpecularParameters();
}

void OpenGlWidget::Shininess(int cl)
{
    LocalScene.shininess = cl;
    InitSpecularParameters();
}

void OpenGlWidget::InitSpecularParameters()
{
    /// For drawing Filled Polygones :
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LocalScene.frontcol);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, LocalScene.backcol);

    glMaterialfv(GL_FRONT, GL_SPECULAR, LocalScene.specReflection);
    glMaterialfv(GL_BACK, GL_SPECULAR, LocalScene.specReflection);

    glMateriali(GL_FRONT, GL_SHININESS, LocalScene.shininess);
    glMateriali(GL_BACK, GL_SHININESS, LocalScene.shininess);
    glEnable(GL_DEPTH_TEST);
    update();
}
