/**
 * @file Platform/Nao/DebugHandler.h
 *
 * Class for debug communication over a TCP connection
 *
 * @author Thomas Röfer
 */

#pragma once

#include "Tools/Debugging/TcpConnection.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include "Representations/BehaviorControl/BlocklyControl.h"

class BlocklyServer : TcpConnection
{
private:
  std::string recieveMessage;
  unsigned char* sendData = nullptr; /**< The data to send next. */
  size_t sendSize = 0; /**< The size of the data to send next. */
  
  std::queue<std::string> blocks; /** Vector that contains the blockly constructions. Each Blockly-Block is an vector in this vectorlist. */
  std::queue<std::string> currentBlock; /** Contains name of current process and all his needed variables */;
  
  void parsing (std::string sequence, bool processSequence);
  void callBlock();

public:
  /**
   * @param in The message queue that stores data received.
   * @param out The message queue containing data to be sent.
   * @param maxPacketSendSize The maximum size of an outgoing packet.
   *                           If 0, this setting is ignored.
   * @param maxPacketReceiveSize The maximum size of an incoming packet.
   *                              If 0, this setting is ignored.
   */
  BlocklyServer(int maxSendSize = 0, int maxReceiveSize = 0);

  /**
   * Delete buffered packet if needed.
   */
  ~BlocklyServer() {if(sendData) delete [] sendData;}

  /**
   * The method performs the communication.
   * It has to be called at the end of each frame.
   * @param send Send outgoing queue?
   */
  void communicate();
 
};
