// header
#include "../VSPluginRobotLanguageTranslatorVisu.h"
// specifc
#include "../../VSLibRenderGL/VSLibRenderGL.h"

#include "../../../Main/VEROSIM/VEROSIMProject.h"
#include "../../../Lib/VSD/VSDDatabase.h"
#include "../../../Lib/VSD/VSDElementIndex.h"
#include "../../../Lib/VSD3D/VSD3DNode.h"
#include "../../../Lib/VSD3D/VSD3DHullNode.h"
#include "../../../Lib/VSD3D/VSD3DGeometrySphere.h"

#include "../../VSLibRenderGL/VSLibRenderGLGLAttribPusher.h"

#include "../VSPluginRobotLanguageTranslatorProject.h"
#include "../VSPluginRobotLanguageTranslatorExtensionRotate.h"
#include "../VSPluginRobotLanguageTranslatorExtensionTranslate.h"



VSPluginRobotLanguageTranslator::Visu::Visu(Project* project)
: VSLibRenderGL::RenderExtension(project->getProject(), tr("VSPluginRotateNodeWithPose::Visu"))
, project(project)
, arrowListIdRot(0)
, arrowListIdTrans(0)
{
   addToLayer(VSLibRenderGL::LAYER_MAIN_EXTENDS);
}

VSPluginRobotLanguageTranslator::Visu::~Visu()
{
   //delete GLList??
}

void VSPluginRobotLanguageTranslator::Visu::render(VSLibRenderGL::RenderOption option)
{
   Q_UNUSED(option);
   // check rendering enabled
   if(!project->getVisuRotAxis())
      return;

   // save current GL attribute state
   // will be restored when leaving scope
   VSLibRenderGL::GLAttribPusher attribPush(GL_ENABLE_BIT);
   glEnable(GL_NORMALIZE);
   
   // first creation of GL display lists
   if (glIsList(arrowListIdRot) == GL_FALSE)
      initArrowList(arrowListIdRot, true);
   if (glIsList(arrowListIdTrans) == GL_FALSE)
      initArrowList(arrowListIdTrans, false);
   
   VSM::Frame hullFrame;
   //render all translating nodes
   VSDForAllElements(VSPluginRobotLanguageTranslator::ExtensionTranslate*, rotateNodeExtension, this->getSimState()->getDatabase())
   {
   
      // get parent node with pose
      VSD3D::Node* extensionParent = rotateNodeExtension->getMyModelInstance()->instanceCast<VSD3D::Node*>();
      if(!extensionParent)
         continue;
      
      QColor color(0, 0, 255);
      // calc arrow length from bounding box
      double arrowLength = 1.5 * (  extensionParent->getAxisAlignedBoundingBox().getMaxPos()
                                   -extensionParent->getAxisAlignedBoundingBox().getMinPos()).length();

      // visu frame
      VSM::Frame currentFrame = extensionParent->compileUpdatedWorldFrame();
      VSM::Frame visuFrame = currentFrame * rotateNodeExtension->getVisuFrame();

      // draw GL display lists at visu frame with given arrow 
      glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
         double m[16];
         visuFrame.copyTo(m);
         glMultMatrixd(m);
         glScaled(arrowLength, arrowLength, arrowLength);
         VSLibRenderGL::setAmbientAndDiffuse(color.red(),color.green(),color.blue());
         glCallList(arrowListIdTrans);
      glPopMatrix();
   }

   // render all rotating node with pose extensions
   VSDForAllElements(VSPluginRobotLanguageTranslator::ExtensionRotate*, rotateNodeExtension, this->getSimState()->getDatabase())
   {
   
      // get parent node with pose
      VSD3D::Node* extensionParent=rotateNodeExtension->getMyModelInstance()->instanceCast<VSD3D::Node*>();
      if(!extensionParent)
         continue; 

      // calc arrow length from bounding box
      QColor color(0, 0, 255);
      double arrowLength = 1.5 * extensionParent->getBoundingSphere().getRadius();

      // visu frame
      VSM::Frame currentFrame = extensionParent->compileUpdatedWorldFrame();
      VSM::Frame visuFrame = currentFrame * rotateNodeExtension->getVisuFrame();

      // draw GL display lists at visu frame with given arrow properties
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
         double m[16];
         visuFrame.copyTo(m);
         glMultMatrixd(m);
         glScaled(arrowLength, arrowLength, arrowLength);
         VSLibRenderGL::setAmbientAndDiffuse(color.red(), color.green(), color.blue());
         glCallList(arrowListIdRot);
      glPopMatrix();
   }
}

void VSPluginRobotLanguageTranslator::Visu::initialize()
{
}

void VSPluginRobotLanguageTranslator::Visu::initArrowList(uint& listID, const bool writeTorus)
{
   // check GL display list exists
   if (glIsList(listID) == GL_TRUE)
   {
      glDeleteLists(listID, 1);
      listID = 0;
   }

   // create GL display list
   listID = glGenLists(1);
   glNewList(listID, GL_COMPILE);
   {
      glDisable(GL_TEXTURE_2D);
      VSLibRenderGL::setEmissionAndSpecular(0, 0, 0);

      glMatrixMode(GL_MODELVIEW);
      
      glPushMatrix();

      writeArrowShaftInList();
      writeArrowTipInList();

      glTranslatef(0.0, 0.0, 0.9f);
      if (writeTorus)
         writeTorusInList();

      glPopMatrix();
   }
   glEndList();
}

void VSPluginRobotLanguageTranslator::Visu::writeArrowShaftInList(double factor)
{
   double longCyl = factor * 0.01 * 0.92388;
   double shortCyl = factor * 0.01 * 0.38268;
   double lengthCyl = 0.8;

   // draw the stem of the arrow as a hexagonal cylinder
   glBegin(GL_QUAD_STRIP);
   {
      glNormal3d(  -0.92388,  -0.38268, 0.0 );
      glVertex3d(  -longCyl, -shortCyl, lengthCyl );
      glVertex3d(  -longCyl, -shortCyl, 0.0 );

      glNormal3d( -0.38268,   -0.92388, 0.0 );
      glVertex3d( -shortCyl,  -longCyl, lengthCyl );
      glVertex3d( -shortCyl,  -longCyl, 0.0 );

      glNormal3d(  0.38268,   -0.92388, 0.0 );
      glVertex3d(  shortCyl,  -longCyl, lengthCyl );
      glVertex3d(  shortCyl,  -longCyl, 0.0 );

      glNormal3d(   0.92388,  -0.38268, 0.0 );
      glVertex3d(   longCyl, -shortCyl, lengthCyl );
      glVertex3d(   longCyl, -shortCyl, 0.0 );

      glNormal3d(   0.92388,   0.38268, 0.0 );
      glVertex3d(   longCyl,  shortCyl, lengthCyl );
      glVertex3d(   longCyl,  shortCyl, 0.0 );

      glNormal3d(  0.38268,    0.92388, 0.0 );
      glVertex3d(  shortCyl,   longCyl, lengthCyl );
      glVertex3d(  shortCyl,   longCyl, 0.0 );

      glNormal3d( -0.38268,    0.92388, 0.0 );
      glVertex3d( -shortCyl,   longCyl, lengthCyl );
      glVertex3d( -shortCyl,   longCyl, 0.0 );

      glNormal3d(  -0.92388,   0.38268, 0.0 );
      glVertex3d(  -longCyl,  shortCyl, lengthCyl );
      glVertex3d(  -longCyl,  shortCyl, 0.0 );

      glNormal3d(  -0.92388,  -0.38268, 0.0 );
      glVertex3d(  -longCyl, -shortCyl, lengthCyl );
      glVertex3d(  -longCyl, -shortCyl, 0.0 );
   }
   glEnd();

   // cap the cylinder on the bottom
   glBegin(GL_TRIANGLE_FAN);
   {
      glNormal3d( 0.0, 0.0, -1.0 );
      glVertex3d(       0.0,       0.0, 0.0);
      glVertex3d(  -longCyl, -shortCyl, 0.0 );
      glVertex3d(  -longCyl,  shortCyl, 0.0 );
      glVertex3d( -shortCyl,   longCyl, 0.0 );
      glVertex3d(  shortCyl,   longCyl, 0.0 );
      glVertex3d(   longCyl,  shortCyl, 0.0 );
      glVertex3d(   longCyl, -shortCyl, 0.0 );
      glVertex3d(  shortCyl,  -longCyl, 0.0 );
      glVertex3d( -shortCyl,  -longCyl, 0.0 );
      glVertex3d(  -longCyl, -shortCyl, 0.0 );
   }
   glEnd();
}

void VSPluginRobotLanguageTranslator::Visu::writeArrowTipInList()
{
   double longNorm = 0.92388;
   double shortNorm = 0.38268;
   double diagNorm= 0.70711;

   double lengthCyl = 0.8;
   double longCone = 0.06 * longNorm;
   double shortCone = 0.06 * shortNorm;

   // draw the tip of the arrow as a hexagonal cone
   glBegin(GL_TRIANGLES);
   {
      //ABC
      glNormal3d( -diagNorm, -diagNorm, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(   -longNorm,   -shortNorm, 0.0 );
      glVertex3d(  -longCone, -shortCone, lengthCyl );
      glNormal3d(   -shortNorm,   -longNorm, 0.0 );
      glVertex3d( -shortCone,  -longCone, lengthCyl );
      //ACD
      glNormal3d( 0.0, -1.0, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(   -shortNorm,   -longNorm, 0.0 );
      glVertex3d( -shortCone,  -longCone, lengthCyl );
      glNormal3d(    shortNorm,   -longNorm, 0.0 );
      glVertex3d(  shortCone,  -longCone, lengthCyl );
      //ADE
      glNormal3d( diagNorm, -diagNorm, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(    shortNorm,   -longNorm, 0.0 );
      glVertex3d(  shortCone,  -longCone, lengthCyl );
      glNormal3d(    longNorm,   -shortNorm, 0.0 );
      glVertex3d(   longCone, -shortCone, lengthCyl );
      //AEF
      glNormal3d( 1.0, 0.0, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(    longNorm,   -shortNorm, 0.0 );
      glVertex3d(   longCone, -shortCone, lengthCyl );
      glNormal3d(    longNorm,    shortNorm, 0.0 );
      glVertex3d(   longCone,  shortCone, lengthCyl );
      //AFG
      glNormal3d( diagNorm, diagNorm, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(    longNorm,    shortNorm, 0.0 );
      glVertex3d(   longCone,  shortCone, lengthCyl );
      glNormal3d(    shortNorm,    longNorm, 0.0 );
      glVertex3d(  shortCone,   longCone, lengthCyl );		
      //AGH
      glNormal3d( 0.0, 1.0, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(    shortNorm,    longNorm, 0.0 );
      glVertex3d(  shortCone,   longCone, lengthCyl );
      glNormal3d(   -shortNorm,    longNorm, 0.0 );
      glVertex3d( -shortCone,   longCone, lengthCyl );
      //AHI
      glNormal3d( -diagNorm, diagNorm, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(   -shortNorm,    longNorm, 0.0 );
      glVertex3d( -shortCone,   longCone, lengthCyl );
      glNormal3d(   -longNorm,    shortNorm, 0.0 );
      glVertex3d(  -longCone,  shortCone, lengthCyl );
      //AIB
      glNormal3d( -1.0, 0.0, 0.0 );
      glVertex3d( 0.0,0.0, 1.0);
      glNormal3d(   -longNorm,    shortNorm, 0.0 );
      glVertex3d(  -longCone,  shortCone, lengthCyl );
      glNormal3d(   -longNorm,   -shortNorm, 0.0 );
      glVertex3d(  -longCone, -shortCone, lengthCyl );
   }
   glEnd();


   // put a bottom on the cone
   glBegin(GL_TRIANGLE_FAN);
   {
      glNormal3d( 0.0, 0.0, -1.0 );
      glVertex3d(        0.0,        0.0, lengthCyl );
      glVertex3d(  -longCone, -shortCone, lengthCyl );
      glVertex3d(  -longCone,  shortCone, lengthCyl );
      glVertex3d( -shortCone,   longCone, lengthCyl );
      glVertex3d(  shortCone,   longCone, lengthCyl );
      glVertex3d(   longCone,  shortCone, lengthCyl );
      glVertex3d(   longCone, -shortCone, lengthCyl );
      glVertex3d(  shortCone,  -longCone, lengthCyl );
      glVertex3d( -shortCone,  -longCone, lengthCyl );
      glVertex3d(  -longCone, -shortCone, lengthCyl );
   }
   glEnd();
}

void VSPluginRobotLanguageTranslator::Visu::writeTorusInList()
{
   double circleRadius = 0.1;
   int circlePoints = 5;

   double torusWidth = 0.01;
   double tipWidth = 2.0 * torusWidth;
   double tipLength = 4.0 * torusWidth;
   int torusSlices = 6;

   // only upper half of torus, since lower half is symmetrical
   VSM::Vector3** coord = new VSM::Vector3*[(3 * circlePoints) + 1];
   for (int a = 0; a <= 3 * circlePoints; a++)
      coord[a] = new VSM::Vector3[torusSlices * 2];

   int h = 0; // "height"
   int s = 0; // "slice"
   for (s = 0; s <= 3 * circlePoints; s++)
   {
      double angle = (2.0 * VSM::pi/(double)(4.0 * circlePoints)) * s;
      VSM::Vector3 centerDirection(cos(angle), sin(angle), 0);
      for (h = 0; h < 2 * torusSlices; h++)
      {
         double torusRad = (VSM::pi/(double)((2.0 * torusSlices) - 1.0)) * h;
         VSM::Vector3 centerPoint = centerDirection * circleRadius;
         double height = sin(torusRad) * torusWidth;
         double distFromCenter = cos(torusRad) * torusWidth;
         VSM::Vector3 addVector = VSM::Vector3( centerDirection.getX() * distFromCenter, 
                                                centerDirection.getY() * distFromCenter, 
                                                height);
         coord[s][h]= centerPoint + addVector;
      }
   }

   for (s = 0; s <= (3 * circlePoints) - 1; s++)
   {
      int nextS = s+1;
      for (h = 0; h < 2 * torusSlices - 1; h++)
      {
         glBegin(GL_TRIANGLES);
         {
            // upper half
            glVertex3d(coord[s][h].getX(),         coord[s][h].getY(),        coord[s][h].getZ());
            glVertex3d(coord[nextS][h+1].getX(),   coord[nextS][h+1].getY(),  coord[nextS][h+1].getZ());
            glVertex3d(coord[s][h+1].getX(),       coord[s][h+1].getY(),      coord[s][h+1].getZ());

            glVertex3d(coord[s][h].getX(),         coord[s][h].getY(),        coord[s][h].getZ());
            glVertex3d(coord[nextS][h].getX(),     coord[nextS][h].getY(),    coord[nextS][h].getZ());
            glVertex3d(coord[nextS][h+1].getX(),   coord[nextS][h+1].getY(),  coord[nextS][h+1].getZ());

            // lower half
            glVertex3d(coord[s][h+1].getX(),	      coord[s][h+1].getY(),	  -coord[s][h+1].getZ());
            glVertex3d(coord[nextS][h].getX(),     coord[nextS][h].getY(),	  -coord[nextS][h].getZ());
            glVertex3d(coord[s][h].getX(),		   coord[s][h].getY(),		  -coord[s][h].getZ());

            glVertex3d(coord[s][h+1].getX(),		   coord[s][h+1].getY(),	  -coord[s][h+1].getZ());
            glVertex3d(coord[nextS][h+1].getX(),   coord[nextS][h+1].getY(), -coord[nextS][h+1].getZ());
            glVertex3d(coord[nextS][h].getX(),	   coord[nextS][h].getY(),	  -coord[nextS][h].getZ());
         }
         glEnd();

      }
   }

   // delete array
   for (int a = 0; a <= 3 * circlePoints; a++)
      delete[] coord[a];
   delete[] coord;

   
   VSM::Vector3 dir1(cos(0.0), sin(0.0), 0);
   double angle = VSM::pi * 1.5;
   VSM::Vector3 dir2(cos(angle), sin(angle), 0);

   VSM::Vector3 centerPoint1;
   VSM::Vector3 centerPoint2;
   VSM::Vector3 tip1;
   VSM::Vector3 tip2;

   VSM::Vector3* points1 = new VSM::Vector3[4 * torusSlices];
   VSM::Vector3* points2 = new VSM::Vector3[4 * torusSlices];

   for (h = 0; h < 4 * torusSlices; h++)
   {
      double torusRad = (VSM::pi/(double)((2.0 * torusSlices) - 1.0)) * h;
      double height = sin(torusRad) * tipWidth;
      double distFromCenter = cos(torusRad) * tipWidth;

      centerPoint1 = dir1 * circleRadius;
      VSM::Vector3 addVector1 = VSM::Vector3(dir1.getX() * distFromCenter, 
                                             dir1.getY() * distFromCenter, 
                                             height);
      points1[h] = centerPoint1 + addVector1;

      centerPoint2 = dir2 * circleRadius;
      VSM::Vector3 addVector2 = VSM::Vector3(dir2.getX() * distFromCenter, 
                                             dir2.getY() * distFromCenter, 
                                             height);
      points2[h] = centerPoint2 + addVector2;

      if (h == 0)
      {
         tip1 = centerPoint1 + dir2 * tipLength;
         tip2 = centerPoint2 + dir1 * tipLength;
      }
   }

   glBegin(GL_TRIANGLE_FAN);
   {
      glVertex3d(centerPoint1.getX(), centerPoint1.getY(), centerPoint1.getZ());
      for (h = (4 * torusSlices) - 1; h > 0; h--)
      {
         glVertex3d(points1[h].getX(), points1[h].getY(), points1[h].getZ());
      }
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   {
      glVertex3d(tip1.getX(), tip1.getY(), tip1.getZ());
      for (h = 0; h < 4 * torusSlices; h++)
      {
         glVertex3d(points1[h].getX(), points1[h].getY(), points1[h].getZ());
      }
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   {
      glVertex3d(centerPoint2.getX(), centerPoint2.getY(), centerPoint2.getZ());
      for (h = 0; h < 4 * torusSlices; h++)
      {
         glVertex3d(points2[h].getX(), points2[h].getY(), points2[h].getZ());
      }
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   {
      glVertex3d(tip2.getX(), tip2.getY(), tip2.getZ());
      for (h = (4 * torusSlices) - 1; h > 0; h--)
      {
         glVertex3d(points2[h].getX(), points2[h].getY(), points2[h].getZ());
      }
   }
   glEnd();

   delete[] points1;
   delete[] points2;
}
