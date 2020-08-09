/**
 * @file BehaviorControl.h
 *
 * This file declares the new BehaviorControl in Season 2019.
 *
 * @author Jesse Richter-Klug
 */

#pragma once

#include "Representations/BehaviorControl/ActivationGraph.h"
#include "Representations/BehaviorControl/BehaviorStatus.h"
#include "Representations/BehaviorControl/Libraries/LibCheck.h"
#include "Representations/Communication/RobotInfo.h"
#include "Representations/Infrastructure/CameraStatus.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/RobotHealth.h"
#include "Representations/Infrastructure/SensorData/KeyStates.h"
#include "Representations/Infrastructure/TeamTalk.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/MotionControl/ArmMotionRequest.h"
#include "Representations/MotionControl/HeadMotionRequest.h"
#include "Representations/MotionControl/MotionRequest.h"
#include "Representations/Infrastructure/SensorData/JointSensorData.h"
#include "Tools/Module/Module.h"
#include "Tools/Streams/Enum.h"
#include <string>
#include "Controller/ConsoleRoboCupCtrl.h"

//ME
#include "Representations/MotionControl/MotionInfo.h"
#include <chrono>
#include "Representations/Modeling/SelfLocalizationHypotheses.h"
#include "Tools/Math/Projection.h"
MODULE(BlocklyControl,
{,
  REQUIRES(CameraStatus),
  REQUIRES(EnhancedKeyStates),
  REQUIRES(RobotHealth),
  REQUIRES(RobotInfo),

  REQUIRES(FrameInfo),
  REQUIRES(JointSensorData),
  REQUIRES(LibCheck),
  REQUIRES(RobotPose),

  PROVIDES(ActivationGraph),
  REQUIRES(ActivationGraph),

  PROVIDES(ArmMotionRequest),
  PROVIDES(BehaviorStatus),
  PROVIDES(HeadMotionRequest),
  PROVIDES(MotionRequest),
  PROVIDES(TeamTalk),

    //ME
  REQUIRES(MotionInfo),
  
  LOADS_PARAMETERS(
  {,
    (std::string) rootCard, /**< The card that is executed directly by this module. */
  }),
});

// Include here so macros do not dismantle themself
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Skill/Skill.h"

class BlocklyControl : public BlocklyControlBase
{
public:
  /** Constructor. */
  BlocklyControl();

  /**
   * Creates extended module info (union of this module's info and requirements of all behavior parts (cards and skills)).
   * @return The extended module info.
   */
  static std::vector<ModuleBase::Info> getExtModuleInfo();

private:
  /**
   * Updates the activation graph.
   * @param activationGraph The provided activation graph.
   */
  void update(ActivationGraph& activationGraph) override;

  /** Executes the top-level state machine of the behavior. */
  void execute(std::string);

  /**
   * Updates the arm motion request.
   * @param armMotionRequest The provided arm motion request.
   */
  void update(ArmMotionRequest& armMotionRequest) override { armMotionRequest = theArmMotionRequest; }

  /**
   * Updates the behavior status.
   * @param behaviorStatus The provided behavior status.
   */
  void update(BehaviorStatus& behaviorStatus) override { behaviorStatus = theBehaviorStatus; }

  /**
   * Updates the head motion request.
   * @param headMotionRequest The provided head motion request.
   */
  void update(HeadMotionRequest& headMotionRequest) override { headMotionRequest = theHeadMotionRequest; }

  /**
   * Updates the motion request.
   * @param motionRequest The provided motion request.
   */
    //void update(MotionRequest& theMotionRequest) override;
  void update(MotionRequest& motionRequest) override { motionRequest = theMotionRequest; }
   
  /**
   * Updates team talk.
   * @param motionRequest The provided team talk.
   */
  void update(TeamTalk& teamTalk) override { teamTalk = theTeamTalk; }

  ENUM(State,
  {,
    inactive,
    sittingDown,
    gettingUp,
    cameraStatusFAILED,
    lowBattery,
    penalized,
    playing,
    
  });

  State status = inactive;

  ArmMotionRequest theArmMotionRequest; /**< The arm motion request that is modified by the behavior. */
  BehaviorStatus theBehaviorStatus; /**< The behavior status that is modified by the behavior. */
  HeadMotionRequest theHeadMotionRequest; /**< The head motion request that is modified by the behavior. */
  MotionRequest theMotionRequest; /**< The motion request that is modified by the behavior. */
  TeamTalk theTeamTalk; /**< The team talk that is modified by the behavior. */

  SkillRegistry theSkillRegistry; /**< The manager of all skills. */
  CardRegistry theCardRegistry; /**< The manager of all cards. */
  
  
  /**Own methods for blockly - motionRequest: */
  void sitDown();
  void walk();
  void walkToTarget(const Pose2f& speed, const Pose2f& target); //TODO BLockly Grid Field
  void walkOverTime();
  void getUp();
  void standHighGetUp();
  void kick(); //TODO KickInWalk?
  void rotateWholeNao();
  void say(const char* text);
  void playSound();
  void rotateHead();
  void rotateArms(); //TODO L/R
  void allStop();
  
/**
 * A status machine, that set the state of the working request.
 * @return True = request is running. False = no request is running.
 */
  bool setStateMachine();
  

  ENUM (StateMachine, // https://stackoverflow.com/questions/28779334/how-to-enable-c11-features-in-codelite 
  {,
    idle,
    stateWalk,
    stateWalkToTarget,
    stateWalkOverTime,
    stateWalkOnPathToTarget,
    stateSitDown,
    stateGetUp,
    stateStandHighGetUp,
    stateKick,
    stateRotateWholeNao,
    stateSay,
    statePlaySound,
    stateRotateHead,
    stateRotateArms,
    statePointArmToTarget,
  });
  
  StateMachine stateMachine = stateWalkOverTime;
  bool complete;
  bool begin;
  Pose2f target = Pose2f(0,0,0);
  

 bool wait();
 /**
  * Give a bool state if the robot is still walking or has finished @code{walkTime}.
  * @return True = if the walking time of the robot has end. False = walking time is still running.
  */
 bool walkTimeIsOver();      
 
 int walkTime;              //Time in Seconds, how long the robot should walk
 bool timeState;            //True = Roboter should walk over time. False = Roboter should not walk over time.
 std::chrono::time_point<std::chrono::system_clock> startTime;  //Start time of timer. Works with moveTime together.


  ENUM (WalkStatus, 
  {,
    noneWalk,
    absoluteWalk,
    relativeWalk,
  });
  
  WalkStatus walkStatus = noneWalk;   //Set the active walk style.

};