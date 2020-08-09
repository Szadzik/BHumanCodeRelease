/**
 * @file Platform/Nao/DebugHandler.cpp
 *
 * Class for debug communication over a TCP connection
 *
 * @author Thomas Röfer
 */

#include "BlocklyServer.h"
#include "Platform/BHAssert.h"
#include "Tools/Streams/OutStreams.h"
#include "Tools/Streams/InStreams.h"
#include <limits>

BlocklyServer::BlocklyServer(int maxSendSize, int maxReceiveSize) :
  TcpConnection(0, 1234, TcpConnection::receiver, maxSendSize, maxReceiveSize)
{ std::clog << "blocklydebug --server--" << std::endl;

    }

void BlocklyServer::parsing(std::string sequence, bool processSequence){
    char delimiter = processSequence ? '#': ':';
    std::queue<std::string>&list = processSequence ? blocks : currentBlock;
    //sequence.erase (std::remove (sequence.begin(), sequence.end(), ' '), sequence.end()); //need algorithm includ
    std::stringstream strm_sequence(sequence);
    std::string tmp;
    while (std::getline(strm_sequence, tmp, delimiter)) {
        if (!tmp.empty()) 
            list.push(tmp);
    }
   std::clog << "******** Block is *********"<< std::endl;
   
   std::queue tmpi= list; //copy the original queue to the temporary queue
    while (!tmpi.empty())
    {
        std::string q_element = tmpi.front();
        std::clog << "elemtn ist BLOCKS_ "<< q_element << std::endl;
        tmpi.pop();
      }  
    }
    
void BlocklyServer::callBlock(){
    std::clog << "in callBlock"<< std::endl;
    int a = blocks.size();
    parsing(blocks.front(), false);
    blocks.pop();
    
    std::queue tmp= currentBlock; //copy the original queue to the temporary queue
    while (!tmp.empty())
    {
        std::string q_element = tmp.front();
        std::clog << "ausgewählter block context " << q_element << std::endl;
        tmp.pop();
        std::string m = currentBlock.front();
        //if(m=="hello")
          //BlocklyControl g = new BehaviorContext();
          //g.execute(currentBlock.front());
        currentBlock.pop();
    } std::clog << "nach whoile " << std::endl;
    //map oder enum
    //std::string job = currentBlock.front();
    //std::clog << "job is leoi"<< job << std::endl;
      std::clog << "größe blocks danach " << blocks.size() << std::endl;
    }


void BlocklyServer::communicate()
{  //std::clog << "in communicate "<< isConnected() << std::endl;
 

    unsigned char* sendData = nullptr;

    int sendSize = 0;
    bool connectionData = true;
    std::string message = "";
    unsigned char* receivedData;
    int receivedSize = 0;

    ASSERT(sendSize <= std::numeric_limits<int>::max());
    bool a = (sendAndReceive(sendData, static_cast<int>(sendSize), receivedData, receivedSize) && sendSize);
   
    
   if (receivedSize == -1){
      parsing(getFullMessage(), true);
       do {                          
                    
  std::clog << "CALLLLLLLLLLLLLLL" << std::endl;
        callBlock();
    } while (blocks.size()> 0);
    
    
    
      
   }
    if(isConnected()){
  
        std::clog << "con: " << connectionData << std::endl;
   
        }
}


