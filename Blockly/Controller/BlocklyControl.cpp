/**
 * @file BlocklyControl.cpp
 *
 * This file implements a module that <#...#>
 *
 * @author Thomas Röfer
 */
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

#include "BlocklyControl.h"

#include "Platform/BHAssert.h"
#include "Platform/SystemCall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Tools/Debugging/Annotation.h"
#include <regex>

#include "Representations/BehaviorControl/Skills.h"

using namespace std;

MAKE_MODULE(BlocklyControl, behaviorControl, BlocklyControl::getExtModuleInfo);

#define SKILL(name) (*theSkillRegistry.getSkill<Skills::name##Skill>(#name))

BlocklyControl::BlocklyControl() :
  theSkillRegistry("skills.cfg", const_cast<ActivationGraph&>(theActivationGraph), theArmMotionRequest, theBehaviorStatus, theHeadMotionRequest, theMotionRequest, theTeamTalk),
  theCardRegistry(const_cast<ActivationGraph&>(theActivationGraph))
{
  std::clog << "setting up status" << std::endl;
  theSkillRegistry.checkSkills(CardCreatorBase::gatherSkillInfo(CardCreatorList<Card>::first));
}

std::vector<ModuleBase::Info> BlocklyControl::getExtModuleInfo()
{
  auto result = BlocklyControl::getModuleInfo();

  SkillImplementationCreatorBase::addToModuleInfo(SkillImplementationCreatorList<Skill>::first, result);
  CardCreatorBase::addToModuleInfo(CardCreatorList<Card>::first, result);

  return result;
}


void BlocklyControl::update(ActivationGraph& activationGraph)
{ 
  activationGraph.graph.clear();
  activationGraph.currentDepth = 0;

  theBehaviorStatus.passTarget = -1;
  theBehaviorStatus.walkingTo = Vector2f::Zero();
  theBehaviorStatus.shootingTo = Vector2f::Zero();

  theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::none;
  theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::none;

  theSkillRegistry.modifyAllParameters();
  theCardRegistry.modifyAllParameters();

  theSkillRegistry.preProcess(theFrameInfo.time);
  theCardRegistry.preProcess(theFrameInfo.time);

  ASSERT(activationGraph.graph.empty());
  activationGraph.graph.emplace_back("BlocklyControl", 0, TypeRegistry::getEnumName(status), theFrameInfo.time, 0, std::vector<std::string>());
  this->execute("");
  activationGraph.graph[0].state = TypeRegistry::getEnumName(status);

  theCardRegistry.postProcess();
  theSkillRegistry.postProcess();

  theLibCheck.performCheck(theMotionRequest);
}

/** OWN */

void BlocklyControl::sitDown(){
    SKILL(Activity)(BehaviorStatus::unknown);
    SKILL(LookForward)();
    SKILL(SpecialAction)(SpecialActionRequest::sitDown);
    }
 
void BlocklyControl::walkToTarget(const Pose2f& speed, const Pose2f& target){
    SKILL(Activity)(BehaviorStatus::unknown);
    SKILL(LookForward)();
    SKILL(WalkToTarget) (speed, target);
}
 
void BlocklyControl::walkOverTime(){
 
 if(!timeState){
    startTime = std::chrono::system_clock::now();
    timeState = true;
 }
    SKILL(Activity)(BehaviorStatus::unknown);
    SKILL(LookForward)();
    SKILL(WalkAtAbsoluteSpeed)(Pose2f(0,100,0));
}

bool BlocklyControl::walkTimeIsOver(){ //TODO template and auto <<
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime);
    //std::clog << "elapse is: " << elapsed_time.count() << std::endl;
    return (elapsed_time.count() >= walkTime) ;
}

void BlocklyControl::say(const char* text){
        SKILL(Say)(text);
    }

void BlocklyControl::rotateWholeNao(){
        SKILL(WalkAtAbsoluteSpeed)(Pose2f(90,0,0));
    }
/**
void setWalkStatus(){
    switch(walkStatus){
        case noneWalk:
            
        case absoluteWalk:
        case relativeWalk:
  }  }*/

bool BlocklyControl::setStateMachine(){
    //std::clog <<"zusatnd ist: " << stateMachine << std::endl;
    switch (stateMachine){
        case idle:
            if (SKILL(Stand).isDone()){
                this->stateMachine = stateSitDown;
                return true;
            }
        case stateWalkToTarget:
            if (SKILL(WalkToTarget).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case stateWalkOverTime:
            if(walkTimeIsOver()){
                this->stateMachine = idle;
                return true;
            }
        case stateSitDown:
            if (SKILL(SpecialAction).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case stateSay:
            if(SKILL(Say).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case statePlaySound:
            if(SKILL(PlaySound).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case stateGetUp:
            if(SKILL(GetUpEngine).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case stateKick:
            if(SKILL(Kick).isDone()){
                this->stateMachine = idle;
                return true;
            }
        case stateRotateWholeNao:
            if(SKILL(WalkAtAbsoluteSpeed).isDone()){
                this->stateMachine = idle;
                return true;
            }
        default: 
            return false;
        }
    }

void BlocklyControl::execute(std::string toDo)
{   if(toDo == "hello")
    stateMachine = stateSitDown;
    //std::clog << theMotionInfo.motion << std::endl;
   // std::clog << "behavior status " << theBehaviorStatus.activity << std::endl;
  //  std::clog << " robo pose? " <<  SelfLocalizationHypotheses.hypotheses[0]<< std::endl; //SelfLocalizationHypotheses::Hypothesis()
    OUTPUT_TEXT("Hallo");
    if(stateMachine == idle){
        SKILL(Stand)();
        setStateMachine();
    } 
    else if(stateMachine == stateWalkToTarget){
        SKILL(Activity)(BehaviorStatus::unknown);
        SKILL(LookForward)();
        SKILL(WalkToTarget) (Pose2f(1,1,1), Pose2f(0,400,0));
        setStateMachine();
    }
    else if (stateMachine == stateWalkOverTime){
        std::clog << "in staeWalk " << std::endl;
        walkTime = 15; //+3 or 4 delay
        walkOverTime();
        setStateMachine();
    }
    else if(stateMachine == stateSitDown){
        SKILL(Activity)(BehaviorStatus::unknown);
        SKILL(LookForward)();
        SKILL(SpecialAction)(SpecialActionRequest::sitDown);
        setStateMachine();
    }
    else if (stateMachine == stateSay){
        say("hallo");
        setStateMachine();
    }
    else if (stateMachine == statePlaySound){
        SKILL(PlaySound)("Nao.wav");
        setStateMachine();
    }
    else if (stateMachine == stateGetUp){
       SKILL(GetUpEngine)();
       setStateMachine();
    }
    else if (stateMachine == stateKick){
        SKILL(Kick)((KickRequest::newKick), (false), (3.2f)); //(bool) mirror, (float) length, (bool)(true) armsBackFix
        setStateMachine();
    }
    else if(stateMachine == stateRotateWholeNao){
        rotateWholeNao();
        setStateMachine();
    }
    
}



   /** 
int euclieanDistance2f(Pose2f target, Pose2f robotPose){
     return  sqrt((target[1] - robotPose[1])^2 + (target[2] - robotPose[2])^2);
    } **/  //euclieanDistance2f(target, LocalRobot.robotPose) <= 0 ? true : false;
    
    
