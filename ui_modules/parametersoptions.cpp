/***************************************************************************
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/
#include "parametersoptions.h"
#include "../fparser/fparser.hh"
#include <QtGui>
#include <QApplication>
#include <math.h>
#include <sstream>
#include <string>
int Threads[3]={8,1,64};
int CalculFactor[3]={4,4,4};
int InitParGrid=50;
int InitIsoGrid=40;
static bool MACOS = false;

Parametersoptions::Parametersoptions()
{
    darkpalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkpalette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    darkpalette.setColor(QPalette::Base, QColor(15, 15, 15));
    darkpalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkpalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    darkpalette.setColor(QPalette::ToolTipText, QColor(255, 255, 255));
    darkpalette.setColor(QPalette::Text, QColor(255, 255, 255));
    darkpalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkpalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    darkpalette.setColor(QPalette::BrightText, QColor(255, 0, 0));
    darkpalette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    darkpalette.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
}

void Parametersoptions::SetStyleAndTheme(QApplication &appli, QString style,
        QString theme)
{
    if (style != "Default")
        appli.setStyle(QStyleFactory::create(style));
    if (theme == "Dark")
    {
        appli.setPalette(darkpalette);
    }
    else if (theme == "MyTheme")
    {
        appli.setPalette(mypalette);
    }
}

void Parametersoptions::ReadConfigFile(QString JsonFile, QJsonObject &js)
{
    QJsonParseError err;
    QString sortie;

    QFile file1(JsonFile);
    if (!file1.exists())
    {
        JsonFile = QFileDialog::getOpenFileName(
                       nullptr, QObject::tr("Open mathmodconfig.js File"), "",
                       QObject::tr("Json (*.js)"));
    }
    if (JsonFile == "")
        JsonFile = ":/mathmodconfig.js";
    QFile file(JsonFile);
    // QFileDevice::Permissions p = file.permissions();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument doc = QJsonDocument::fromJson(
                                ((file.readAll()).trimmed())
                                .replace("\n", "")
                                .replace("\t", "")
                                .replace("DOTSYMBOL", dotsymbol.toStdString().c_str()),
                                &err);
        if (err.error)
        {
            QMessageBox message;
            message.setWindowTitle("Error at : " + JsonFile);
            file.close();
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            sortie = (file.readAll());
            int before, after;
            if (sortie.length() > (err.offset + 30))
                after = 30;
            else
                after = sortie.length() - err.offset;
            sortie.truncate(err.offset + after);
            if (err.offset - 30 > 0)
                before = 30;
            else
                before = 0;
            sortie = sortie.remove(0, err.offset - before);
            sortie.replace("\t", " ");
            sortie.replace("\n", " ");
            sortie.insert(before, " >>> Error <<< ");
            message.setText("Error : " + err.errorString() +
                            " at position: " + QString::number(err.offset) +
                            "\n\n***********\n" + "..." + sortie + "...");
            message.adjustSize();
            message.exec();
            file.close();
            return;
        }
        js = doc.object();
        file.close();
    }
    return;
}

void Parametersoptions::ReadJsonFile(QString JsonFile, QJsonObject &js)
{
    QJsonParseError err;
    QString sortie;
    QFile file1(JsonFile);
    if (!file1.exists())
    {
        JsonFile = QFileDialog::getOpenFileName(
                       nullptr, QObject::tr("Open mathmodconfig.js File"), "",
                       QObject::tr("Json (*.js)"));
    }
    QFile file(JsonFile);
    // QFileDevice::Permissions p = file.permissions();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument doc = QJsonDocument::fromJson(
                                ((file.readAll()).trimmed())
                                .replace("\n", "")
                                .replace("\t", "")
                                .replace("DOTSYMBOL", dotsymbol.toStdString().c_str()),
                                &err);
        if (err.error)
        {
            QMessageBox message;
            message.setWindowTitle("Error at : " + JsonFile);
            file.close();
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            sortie = (file.readAll());
            int before, after;
            if (sortie.length() > (err.offset + 30))
                after = 30;
            else
                after = sortie.length() - err.offset;
            sortie.truncate(err.offset + after);
            if (err.offset - 30 > 0)
                before = 30;
            else
                before = 0;
            sortie = sortie.remove(0, err.offset - before);
            sortie.replace("\t", " ");
            sortie.replace("\n", " ");
            sortie.insert(before, " >>> Error <<< ");
            message.setText("Error : " + err.errorString() +
                            " at position: " + QString::number(err.offset) +
                            "\n\n***********\n" + "..." + sortie + "...");
            message.adjustSize();
            message.exec();
            file.close();
            return;
        }
        js = doc.object();
        file.close();
    }
    return;
}

void Parametersoptions::ReadCollectionFile(QString JsonFileName,
        QJsonObject &js)
{
    QJsonParseError err;
    QString sortie;
    QFile JsonFile(JsonFileName);
    if (!JsonFile.exists())
    {
        QFile file2(":/mathmodcollection_empty.js");
        file2.copy(JsonFileName);
        QFile::setPermissions(JsonFileName,
                              QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
    QFile file(JsonFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument doc = QJsonDocument::fromJson(
                                ((file.readAll()).trimmed())
                                .replace("\n", "")
                                .replace("\t", "")
                                .replace("DOTSYMBOL", dotsymbol.toStdString().c_str()),
                                &err);
        if (err.error)
        {
            QMessageBox message;
            message.setWindowTitle("Error at : " + JsonFileName);
            file.close();
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            sortie = (file.readAll());
            int before, after;
            if (sortie.length() > (err.offset + 30))
                after = 30;
            else
                after = sortie.length() - err.offset;
            sortie.truncate(err.offset + after);
            if (err.offset - 30 > 0)
                before = 30;
            else
                before = 0;
            sortie = sortie.remove(0, err.offset - before);
            sortie.replace("\t", " ");
            sortie.replace("\n", " ");
            sortie.insert(before, " >>> Error <<< ");
            message.setText("Error : " + err.errorString() +
                            " at position: " + QString::number(err.offset) +
                            "\n\n***********\n" + "..." + sortie + "...");
            message.adjustSize();
            message.exec();
            file.close();
            return;
        }
        js = doc.object();
        file.close();
    }
    return;
}
void Parametersoptions::maxisogri_valueChanged(int value)
{
    IsoMaxGrid = value;
    QJsonObject tmp = JConfig["Parameters"].toObject();
    tmp["IsoMaxGrid"] = value;
    JConfig["Parameters"] = tmp;
}

void Parametersoptions::maxpargri_valueChanged(int value)
{
    ParMaxGrid = value;
    QJsonObject tmp = JConfig["Parameters"].toObject();
    tmp["ParMaxGrid"] = value;
    JConfig["Parameters"] = tmp;
}

void Parametersoptions::on_loadconfig_clicked()
{
    ReadConfigFile(fullpath, JConfig);
}

ListeModelTexture
Parametersoptions::LoadCollectionModels(QJsonObject &Jcollection,
                                        jcollection &pariso, int argc)
{

    QFile fconllect(filecollection);
    if (fconllect.exists() && ((argc > 1) || MACOS))
        ReadJsonFile(filecollection, Collection);
    else
        ReadJsonFile(":/mathmodcollection.js", Collection);

    Jcollection = Collection;
    QJsonArray array = Collection["MathModels"].toArray();
    QStringList lst;
    QJsonArray array2 = pariso.JTextures = Collection["TexturesList"].toArray();

    QJsonArray array3 = pariso.JPigments = Collection["PigmentsList"].toArray();

    QStringList lst_2, lst_3;
    QString a;
    QJsonObject jsobj;
    QJsonArray  jsarray;
    // Textures List:
    for (int i = 0; i < array2.size(); i++)
    {
        if ((array2[i].toObject())["Texture"].isObject())
        {
            jsobj = (array2[i].toObject())["Texture"].toObject();
            a = (jsobj)["Name"].toString();
            lst_2.append(a);
        }
    }
    lst_2.insert(0, "Textures List (" + QString::number(lst_2.count()) + ")");

    // Pigments List:
    for (int i = 0; i < array3.size(); i++)
    {
        if ((array3[i].toObject())["Pigment"].isObject())
        {
            jsobj = (array3[i].toObject())["Pigment"].toObject();
            a = (jsobj)["Name"].toString();
            lst_3.append(a);
        }
    }
    lst_3.insert(0, "PigmentsList (" + QString::number(lst_3.count()) + ")");

    for (int i = 0; i < array.size(); i++)
    {
        if ((array[i].toObject())["Iso3D"].isObject())
        {
            jiso newjiso;
            jsobj = (array[i].toObject())["Iso3D"].toObject();
            newjiso.read(jsobj);
            pariso.JIso.append(newjiso);
            a = (jsobj)["Name"].toArray()[0].toString();
            lst.append(a);
        }
        else if ((array[i].toObject())["Param3D"].isObject())
        {
            jpar newjpar;
            jsobj = ((array[i].toObject())["Param3D"].toObject());
            newjpar.read(jsobj);
            pariso.JPar.append(newjpar);
            a = (jsobj)["Name"].toArray()[0].toString();
            lst.append(a);
        }
        else if ((array[i].toObject())["Param4D"].isObject())
        {
            jpar newjpar;
            jsobj = ((array[i].toObject())["Param4D"].toObject());
            newjpar.read(jsobj);
            pariso.JPar.append(newjpar);
            a = (jsobj)["Name"].toArray()[0].toString();
            lst.append(a);
        }
        else if ((array[i].toObject())["Param3D_C"].isObject())
        {
            jpar newjpar;
            jsobj = ((array[i].toObject())["Param3D_C"].toObject());
            newjpar.read(jsobj);
            pariso.JPar.append(newjpar);
            a = (jsobj)["Name"].toArray()[0].toString();
            lst.append(a);
        }
        else if ((array[i].toObject())["Param4D_C"].isObject())
        {
            jpar newjpar;
            jsobj = ((array[i].toObject())["Param4D_C"].toObject());
            newjpar.read(jsobj);
            pariso.JPar.append(newjpar);
            a = (jsobj)["Name"].toArray()[0].toString();
            lst.append(a);
        }
        else if ((array[i].toObject())["ParIso"].isArray())
        {
            jpariso newjpariso;
            jpar newjpar;
            jiso newjiso;
            jsarray = ((array[i].toObject())["ParIso"].toArray());
            for(int i=0; i<jsarray.size(); i++)
            {
                if ((jsarray[i].toObject())["Param3D"].isObject())
                {
                   jsobj =  (jsarray[i].toObject())["Param3D"].toObject();
                   newjpar.read(jsobj);
                   newjpariso.JPar = newjpar;
                }
                else if ((jsarray[i].toObject())["Param3D_C"].isObject())
                {
                   jsobj =  (jsarray[i].toObject())["Param3D_C"].toObject();
                   newjpar.read(jsobj);
                   newjpariso.JPar = newjpar;
                }
                else if ((jsarray[i].toObject())["Param4D_C"].isObject())
                {
                   jsobj =  (jsarray[i].toObject())["Param4D_C"].toObject();
                   newjpar.read(jsobj);
                   newjpariso.JPar = newjpar;
                }
                else if ((jsarray[i].toObject())["Iso3D"].isObject())
                {
                   jsobj =  (jsarray[i].toObject())["Iso3D"].toObject();
                   newjiso.read(jsobj);
                   newjpariso.JIso = newjiso;
                }
            }
            a = newjpariso.Name = (array[i].toObject())["Name"].toString();
            pariso.JParIso.append(newjpariso);
            lst.append(a);
        }
    }

    lst.sort();
    lst.insert(0, "Examples (" + QString::number(lst.count()) + ")");

    ListeModelTexture str;
    str.listeModels = lst;
    str.listeTextures = lst_2;
    str.listePigments = lst_3;
    return str;
}

bool Parametersoptions::isFloat(std::string myString)
{
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

void Parametersoptions::LoadConfig(QApplication &app, int argc, char *argv[])
{
    QString argv1="";
    if (argc > 1)
    {
        argv1 = QString(argv[1]);
        if (!argv1.endsWith("/"))
            argv1 += "/";
        filecollection = argv1 + filecollection;
        fileconfig = argv1 + fileconfig;
        advancedmodels = argv1 + advancedmodels;
    }
    /*
    #ifdef Q_OS_MACOS
        else
      {
            MACOS = true;
            QString appDirPath = QApplication::applicationDirPath();
            appDirPath = appDirPath.remove(fileconfig2.size()-5, 5);
            filecollection   = appDirPath + "Resources/" + filecollection;
            fileconfig       = appDirPath + "Resources/" + fileconfig;
            advancedmodels   = appDirPath + "Resources/" + advancedmodels;
         }
    #endif
    */
    QFile mathmodfileconfig(fileconfig);
    if (!mathmodfileconfig.exists() && ((argc > 1) || MACOS))
    {
        QFile file3(":/mathmodconfig.js");
        file3.copy(fileconfig);
        QFile::setPermissions(fileconfig,
                              QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    QFile fconf(fileconfig);
    if (fconf.exists())
        ReadJsonFile(fileconfig, JConfig);
    else
        ReadJsonFile(":/mathmodconfig.js", JConfig);

    {
        if (JConfig["Localization"].isObject())
        {
            QJsonObject tmp;
            tmp = JConfig["Localization"].toObject();
            if (tmp["AutomaticDetection"].isBool() &&
                    tmp["AutomaticDetection"].toBool())
            {
                FunctionParser ff;
                if ((ff.Parse("1.02", "t")) >= 0)
                    dotsymbol = ",";
                else
                    dotsymbol = ".";
            }
            else
                dotsymbol = tmp["DotSymbol"].toString();
        }

        if (JConfig["StartOptions"].isObject())
        {
            QJsonObject tmp;
            tmp = JConfig["StartOptions"].toObject();
            model = tmp["Model"].toString();
        }

        if (JConfig["Parameters"].isObject())
        {
            IsoParam = JConfig["Parameters"].toObject();
            if ((IsoParam)["IsoMaxGrid"].isDouble())
                IsoMaxGrid = ((IsoParam)["IsoMaxGrid"]).toInt();

            if ((IsoParam)["ParMaxGrid"].isDouble())
                ParMaxGrid = ((IsoParam)["ParMaxGrid"]).toInt();

            if ((IsoParam)["InitParGrid"].isDouble())
                InitParGrid = (IsoParam)["InitParGrid"].toInt();

            if ((IsoParam)["InitIsoGrid"].isDouble())
                InitIsoGrid = (IsoParam)["InitIsoGrid"].toInt();
        }

        QJsonObject tmp;
        if (JConfig["WindowPosition"].isObject())
        {
            tmp = JConfig["WindowPosition"].toObject();
            ControlX = tmp["ControlX"].toInt();
            ControlY = tmp["ControlY"].toInt();
            GlwinX = tmp["GlwinX"].toInt();
            GlwinY = tmp["GlwinY"].toInt();
        }

        if (JConfig["WindowSize"].isObject())
        {
            tmp = JConfig["WindowSize"].toObject();
            ControlW = tmp["ControlW"].toInt();
            ControlH = tmp["ControlH"].toInt();
            GlwinW = tmp["GlwinW"].toInt();
            GlwinH = tmp["GlwinH"].toInt();
        }

        if (JConfig["OpenGlConfig"].isObject())
        {
            QJsonObject tmp1, tmp2;
            tmp1 = JConfig["OpenGlConfig"].toObject();

            Shininess = tmp1["GL_SHININESS"].toInt();

            if (tmp1["GL_FrontFacingSupport"].isBool())
                glFrontFacingSupport = tmp1["GL_FrontFacingSupport"].toBool();

            tmp2 = JConfig["OpenGlConfig"].toObject();
            if (tmp2["GL_SPECULAR"].isArray())
            {
                Specular[0] =
                    (tmp2["GL_SPECULAR"].toArray())[0].toInt();
                Specular[1]=(tmp2["GL_SPECULAR"].toArray())[1].toInt();
                Specular[2]=(tmp2["GL_SPECULAR"].toArray())[2].toInt();
                Specular[3]=(tmp2["GL_SPECULAR"].toArray())[3].toInt();
            }

            if (tmp2["GL_AMBIENT"].isArray())
            {
                Ambient[0]=(tmp2["GL_AMBIENT"].toArray())[0].toInt();
                Ambient[1]=(tmp2["GL_AMBIENT"].toArray())[1].toInt();
                Ambient[2]=(tmp2["GL_AMBIENT"].toArray())[2].toInt();
                Ambient[3]=(tmp2["GL_AMBIENT"].toArray())[3].toInt();
            }

            if (tmp2["GL_DIFFUSE"].isArray())
            {
                Diffuse[0]=(tmp2["GL_DIFFUSE"].toArray())[0].toInt();
                Diffuse[1]=(tmp2["GL_DIFFUSE"].toArray())[1].toInt();
                Diffuse[2]=(tmp2["GL_DIFFUSE"].toArray())[2].toInt();
                Diffuse[3]=(tmp2["GL_DIFFUSE"].toArray())[3].toInt();
            }
        }

        if (JConfig["CalculFactorConfig"].isObject())
        {
            QJsonObject tmp = JConfig["CalculFactorConfig"].toObject();
            CalculFactor[0] = tmp["FactX"].toInt();
            CalculFactor[1] = tmp["FactY"].toInt();
            CalculFactor[2] = tmp["FactZ"].toInt();
        }

        if (JConfig["ThreadsConfig"].isObject())
        {
            QJsonObject tmp = JConfig["ThreadsConfig"].toObject();
            if (tmp["AutoDetect"].isBool() && tmp["AutoDetect"].toBool())
                Threads[0] = QThread::idealThreadCount();
            else
                Threads[0] = tmp["ThreadsNumber"].toInt();
            Threads[1] = tmp["MinThreadsNumber"].toInt();
            Threads[2] = tmp["MaxThreadsNumber"].toInt();
        }

        if (JConfig["ReleaseInfos"].isObject())
        {
            QJsonObject tmp = JConfig["ReleaseInfos"].toObject();
            docpartialpath = tmp["DocPartialPath"].toString();
            docabsolutepath = tmp["DocAbsolutePath"].toString();
            version = tmp["VersionNumber"].toString();
        }
        app.setStyle(QStyleFactory::create("Fusion"));
        app.setPalette(darkpalette);
    }
    fullpath = fileconfig;

    QFile advancedmodelsfile(advancedmodels);
    if (!advancedmodelsfile.exists() && ((argc > 1) || MACOS))
    {
        QFile file2(":/advancedmodels.js");
        QString str;
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file2);
        str.append(stream.readAll());
        str.replace("DOTSYMBOL", dotsymbol.toStdString().c_str());

        QFile file(advancedmodels);
        if (!file.open(QIODevice::WriteOnly))
        {
            std::cerr << "Cannot open file for writing: "
                      << qPrintable(file.errorString()) << std::endl;
            return;
        }
        QTextStream out(&file);
        out << str <<Qt::endl;
        file.close();
        file.copy(advancedmodels);
        QFile::setPermissions(advancedmodels,
                              QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }

    QFile mathmodfile(filecollection);
    if (!mathmodfile.exists() && ((argc > 1) || MACOS))
    {
        QFile file2(":/mathmodcollection.js");
        QString str;
        file2.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file2);
        str.append(stream.readAll());
        str.replace("DOTSYMBOL", dotsymbol.toStdString().c_str());
        QFile file(filecollection);
        if (!file.open(QIODevice::WriteOnly))
        {
            std::cerr << "Cannot open file for writing: "
                      << qPrintable(file.errorString()) << std::endl;
            return;
        }
        QTextStream out(&file);
        out << str <<Qt::endl;
        file.close();
        file.copy(filecollection);
        QFile::setPermissions(filecollection,
                              QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}
