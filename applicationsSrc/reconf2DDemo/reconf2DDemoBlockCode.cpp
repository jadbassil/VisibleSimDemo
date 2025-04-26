#include "reconf2DDemoBlockCode.hpp"

#include "robots/hexanodes/hexanodesMotionEngine.h"
#include "robots/hexanodes/hexanodesMotionEvents.h"
using namespace Hexanodes;

Reconf2DDemoBlockCode::Reconf2DDemoBlockCode(HexanodesBlock *host) : HexanodesBlockCode(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (handleSampleMessage) to the message of type SAMPLE_MSG_ID
    addMessageEventFunc2(GO_MSG_ID, std::bind(&Reconf2DDemoBlockCode::handleGoMessage, this,
                                              std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(BACK_MSG_ID, std::bind(&Reconf2DDemoBlockCode::handleBackMessage, this,
                                                std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(NOTIFY_MSG_ID, std::bind(&Reconf2DDemoBlockCode::handleNotifyMessage, this,
                                                  std::placeholders::_1, std::placeholders::_2));

    // Set the module pointer
    module = static_cast<HexanodesBlock *>(hostBlock);
}

void Reconf2DDemoBlockCode::startup() {
    console << "start";

    // Sample distance coloring algorithm below
    if (isLeader) {  // Master ID  (check config file)
        module->setColor(RED);
        // Set the distance to 0 and initiate path search

    } else {
        distance = -1;  // Unknown distance
        module->setColor(WHITE);
    }

}

void Reconf2DDemoBlockCode::handleGoMessage(std::shared_ptr<Message> _msg,
                                        P2PNetworkInterface *sender) {
    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());

    int d = *msg->getData() + 1;
    console << " received d =" << d << " from " << sender->getConnectedBlockId() << "\n";
    // Broadcast to all neighbors but ignore sender
    // Choose the parent with the smallest distance to root
    
}

void Reconf2DDemoBlockCode::handleBackMessage(std::shared_ptr<Message> _msg,
                                          P2PNetworkInterface *sender) {
    MessageOf<pair<int, bool>> *msg = static_cast<MessageOf<pair<int, bool>> *>(_msg.get());
    int senderDistanceToRoot = msg->getData()->first;
    bool senderOnPath = msg->getData()->second;
    console << " received back from " << sender->getConnectedBlockId()
            << " d = " << senderDistanceToRoot << "\n";
    // Wait for all answers
    // save the interface that leads to the next moving module
    // If not root send back to parent
    // If root send notify throughout the path to reach the next moving module

}

void Reconf2DDemoBlockCode::handleNotifyMessage(std::shared_ptr<Message> _msg,
                                            P2PNetworkInterface *sender) {
    cout << " received notify from " << sender->getConnectedBlockId() << "\n";
    module->setColor(GREY);
    // forward notify to next in path until reaching the next moving module
    // If the message reaches the next moving module, it will start rotating clockwise

}

void Reconf2DDemoBlockCode::onMotionEnd() {
    // handle motion event
    // continue moving until reaching the target position
    console << " has reached its destination" << "\n";
    if (target->isInTarget(module->position)) {
        console << " has reached its target" << "\n";
        module->setColor(RED);
        // reinitialize local variables
        // Initiate the search for the next path


    } else {
        console << " has not reached its target" << "\n";
        // contiue moving
    }
}

void Reconf2DDemoBlockCode::processLocalEvent(EventPtr pev) {
    std::shared_ptr<Message> message;
    stringstream info;

    // Do not remove line below
    HexanodesBlockCode::processLocalEvent(pev);

    switch (pev->eventType) {
        case EVENT_ADD_NEIGHBOR: {
            // Do something when a neighbor is added to an interface of the module
            break;
        }

        case EVENT_REMOVE_NEIGHBOR: {
            // Do something when a neighbor is removed from an interface of the module
            break;
        }
    }
}

/// ADVANCED BLOCKCODE FUNCTIONS BELOW

void Reconf2DDemoBlockCode::onBlockSelected() {
    // Debug stuff:
    cerr << endl << "--- PRINT MODULE " << *module << "---" << endl;
}

void Reconf2DDemoBlockCode::onAssertTriggered() {
    console << " has triggered an assert" << "\n";

    // Print debugging some info if needed below
    // ...
}

bool Reconf2DDemoBlockCode::parseUserCommandLineArgument(int &argc, char **argv[]) {
    /* Reading the command line */
    if ((argc > 0) && ((*argv)[0][0] == '-')) {
        switch ((*argv)[0][1]) {
            // Single character example: -b
            case 'b': {
                cout << "-b option provided" << endl;
                return true;
            } break;

            // Composite argument example: --foo 13
            case '-': {
                string varg = string((*argv)[0] + 2);  // argv[0] without "--"
                if (varg == string("foo")) {           //
                    int fooArg;
                    try {
                        fooArg = stoi((*argv)[1]);
                        argc--;
                        (*argv)++;
                    } catch (std::logic_error &) {
                        stringstream err;
                        err << "foo must be an integer. Found foo = " << argv[1] << endl;
                        throw CLIParsingError(err.str());
                    }

                    cout << "--foo option provided with value: " << fooArg << endl;
                } else
                    return false;

                return true;
            }

            default:
                cerr << "Unrecognized command line argument: " << (*argv)[0] << endl;
        }
    }

    return false;
}

string Reconf2DDemoBlockCode::onInterfaceDraw() {
    stringstream trace;
    trace << "Some value " << 123;
    return trace.str();
}

void Reconf2DDemoBlockCode::parseUserBlockElements(TiXmlElement *config) {
    const char *attr = config->Attribute("leader");
    if (attr != nullptr) {
        std::cout << getId() << " is leader!" << std::endl;  // complete with your code
        isLeader = true;
    }
}