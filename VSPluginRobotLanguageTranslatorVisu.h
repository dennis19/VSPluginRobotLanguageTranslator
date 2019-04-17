#ifndef VSPluginRotateNodeWithPoseVisuH
#define VSPluginRotateNodeWithPoseVisuH

// base
#include "../VSLibRenderGL/VSLibRenderGLRenderExtension.h"

// specific
//#include "../../Lib/VSM/VSMFrame.h"


namespace VSPluginRobotLanguageTranslator
{
   class Project;

   class Visu : public VSLibRenderGL::RenderExtension 
   {
   
   // construction      
   public:
      Visu(Project* project);
      ~Visu();

   // services      
   public:
      virtual void render(VSLibRenderGL::RenderOption option);
      virtual void initialize();

   // management      
   private:
      void initArrowList(uint& listID, const bool writeTorus);
      void addArrowTorusToList();
      void writeArrowShaftInList(double factor = 1.0);
      void writeArrowTipInList();
      void writeTorusInList();

   // data	   
   private:
      Project* project;
      uint arrowListIdRot;
      uint arrowListIdTrans;
   
   }; // class Visu
}; // namespace VSPluginRotateNodeWithPose

#endif //VSPluginRotateNodeWithPoseVisuH
