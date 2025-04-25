#include "reconf2DBlockCode.hpp"

#include "robots/hexanodes/hexanodesMotionEngine.h"
#include "robots/hexanodes/hexanodesMotionEvents.h"
using namespace Hexanodes;

Reconf2DBlockCode::Reconf2DBlockCode(HexanodesBlock *host) : HexanodesBlockCode(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (handleSampleMessage) to the message of type SAMPLE_MSG_ID
    addMessageEventFunc2(GO_MSG_ID, std::bind(&Reconf2DBlockCode::handleGoMessage, this,
                                              std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(BACK_MSG_ID, std::bind(&Reconf2DBlockCode::handleBackMessage, this,
                                                std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(NOTIFY_MSG_ID, std::bind(&Reconf2DBlockCode::handleNotifyMessage, this,
                                                  std::placeholders::_1, std::placeholders::_2));

    // Set the module pointer
    module = static_cast<HexanodesBlock *>(hostBlock);
}

void Reconf2DBlockCode::startup() {
    console << "start";

    // Sample distance coloring algorithm below
    if (isLeader) {  // Master ID  (check config file)
        module->setColor(RED);
        distance = 0;
        nbWaitedAnswers = sendMessageToAllNeighbors(
            "Go Broadcast", new MessageOf<int>(GO_MSG_ID, distance), 100, 200, 0);
    } else {
        distance = -1;  // Unknown distance
        module->setColor(WHITE);
    }

    // Additional initialization and algorithm start below
    // ...
}

void Reconf2DBlockCode::handleGoMessage(std::shared_ptr<Message> _msg,
                                        P2PNetworkInterface *sender) {
    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());

    int d = *msg->getData() + 1;
    console << " received d =" << d << " from " << sender->getConnectedBlockId() << "\n";

    if (parent == nullptr || distance > d) {
        // reinitialize the path search
        nextInPath = nullptr;
        maxDistance = 0;

        // Update distance and color
        parent = sender;  // Set the parent. It is updated only if the distance is smaller.
        console << " updated distance = " << d << "\n";
        distance = d;
        hostBlock->setColor(Colors[distance % NB_COLORS]);

        // Broadcast to all neighbors but ignore sender
        nbWaitedAnswers = sendMessageToAllNeighbors(
            "Go Broadcast", new MessageOf<int>(GO_MSG_ID, distance), 100, 200, 1, sender);

        if (nbWaitedAnswers == 0) {
            // vector<HexanodesMotion *> tab =
            // Hexanodes::getWorld()->getAllMotionsForModule(module); console << " nb motions = " <<
            // tab.size() << "\n";
            bool canMoveinCW =
                canMove(Hexanodes::motionDirection::CW) && !target->isInTarget(module->position);
            sendMessage(
                "Back",
                new MessageOf<pair<int, bool>>(BACK_MSG_ID, make_pair(distance, canMoveinCW)),
                parent, 100, 200);
        }
    } else {
        // Ignore the message
        sendMessage("Back", new MessageOf<pair<int, bool>>(BACK_MSG_ID, make_pair(distance, false)),
                    sender, 100, 200);
        console << " ignored distance = " << d << "\n";
    }
}

void Reconf2DBlockCode::handleBackMessage(std::shared_ptr<Message> _msg,
                                          P2PNetworkInterface *sender) {
    MessageOf<pair<int, bool>> *msg = static_cast<MessageOf<pair<int, bool>> *>(_msg.get());
    int senderDistanceToRoot = msg->getData()->first;
    bool senderOnPath = msg->getData()->second;
    console << " received back from " << sender->getConnectedBlockId()
            << " d = " << senderDistanceToRoot << "\n";

    nbWaitedAnswers--;
    if (senderDistanceToRoot > maxDistance and senderOnPath) {
        maxDistance = senderDistanceToRoot;
        nextInPath = sender;
    }
    if (nbWaitedAnswers == 0) {
        if (parent != nullptr) {
            // vector<HexanodesMotion *> tab =
            // Hexanodes::getWorld()->getAllMotionsForModule(module); Send back to parent
            bool canMoveInCW = (canMove(Hexanodes::motionDirection::CW) &&
                                !target->isInTarget(module->position)) ||
                               maxDistance > 0;
            if (distance > maxDistance) {
                maxDistance = distance;
                nextInPath = nullptr;
            }
            console << "nbWaitedAnswers = " << nbWaitedAnswers << "\n";
            sendMessage(
                "Back",
                new MessageOf<pair<int, bool>>(BACK_MSG_ID, make_pair(maxDistance, canMoveInCW)),
                parent, 100, 200);
            parent = nullptr;
            distance = -1;
        } else {
            cout << "path found\n";
            if (nextInPath != nullptr) {
                module->setColor(RED);
                // Send notify to next in path
                console << "nextInPath is set " << distance << "\n";
                sendMessage("Notify", new Message(NOTIFY_MSG_ID), nextInPath, 100, 200);
                nextInPath = nullptr;
            } else {
                cout << "nextInPath is null " << distance << "\n";
            }
        }
    }
}

void Reconf2DBlockCode::handleNotifyMessage(std::shared_ptr<Message> _msg,
                                            P2PNetworkInterface *sender) {
    cout << " received notify from " << sender->getConnectedBlockId() << "\n";
    module->setColor(GREY);
    if (nextInPath != nullptr) {
        sendMessage("Notify", new Message(NOTIFY_MSG_ID), nextInPath, 100, 200);
        nextInPath = nullptr;
    } else {
        console << "can start moving\n";
        if (canMove(motionDirection::CW)) moveTo(motionDirection::CW, 500000);
    }
}

void Reconf2DBlockCode::onMotionEnd() {
    console << " has reached its destination" << "\n";
    if (target->isInTarget(module->position)) {
        console << " has reached its target" << "\n";
        module->setColor(RED);
        distance = 0;
        nextInPath = nullptr;
        maxDistance = 0;
        parent = nullptr;
        nbWaitedAnswers = sendMessageToAllNeighbors(
            "Go Broadcast", new MessageOf<int>(GO_MSG_ID, distance), 100, 200, 0);
    } else {
        console << " has not reached its target" << "\n";
        if (canMove(motionDirection::CW)) moveTo(motionDirection::CW, 10000);
    }
    // do stuff
    // ...
}

void Reconf2DBlockCode::processLocalEvent(EventPtr pev) {
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

void Reconf2DBlockCode::onBlockSelected() {
    // Debug stuff:
    cerr << endl << "--- PRINT MODULE " << *module << "---" << endl;
}

void Reconf2DBlockCode::onAssertTriggered() {
    console << " has triggered an assert" << "\n";

    // Print debugging some info if needed below
    // ...
}

bool Reconf2DBlockCode::parseUserCommandLineArgument(int &argc, char **argv[]) {
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

string Reconf2DBlockCode::onInterfaceDraw() {
    stringstream trace;
    trace << "Some value " << 123;
    return trace.str();
}

void Reconf2DBlockCode::parseUserBlockElements(TiXmlElement *config) {
    const char *attr = config->Attribute("leader");
    if (attr != nullptr) {
        std::cout << getId() << " is leader!" << std::endl;  // complete with your code
        isLeader = true;
    }
}