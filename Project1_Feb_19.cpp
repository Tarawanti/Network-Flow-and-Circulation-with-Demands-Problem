/* This program implements Breadth First Search algorithm. The input to the BFS is an Adjacency List. The input to the program is a text file and reads the values from the file to Adjacency List */

/* Header to include library files */
#include <stdlib.h>
#include <conio.h>
#include <sstream>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <exception>

using namespace std;

 typedef struct adjnodes{                                                // Struct declaration for storing the values of weight and adjacent nodes
        int weight;                                                     // stores the value of the weight of the edge
        int nodevalue;                                                 // stores the value of the edge node
        adjnodes * next;                                              // pointer to the next adjnodes
    }ADJNODES;

    typedef struct nodes{                                           //Struct declaration for storing the current node values and its adjacent node list
        int nodevalue;                                             //stores the current node value
        adjnodes * adjpointer;                                    // pointer to the list of the adjacent nodes
        nodes * down;                                            // pointer to the next nodes
    }NODES;

    typedef struct graph{                                       //Struct to store the graph in an array so that the node can be retrieved with indexing
        int numberofnodes ;                                //stores the value for the total number of nodes in the input graph.
        nodes * graphnodes [];                                 // Array of node pointers to store the address of each node
    }GRAPH;

    typedef struct bfslist{                                    //Struct to store the nodes after BFS implementation
        int nodevalue;                                        //Stores the node value
        int parentnode;                                      //Stores the parent node value
        int distance;                                       //Stores the distance from Node 0
        bfslist * next;                                    //pointer to the next bfstree object

    }BFSLIST;

    typedef struct shortpath{                           //Struct to store the nodes retrieving the shortest path from BFSTree
        int node;                                      //Stores the node value
        shortpath * next;                             //pointer to the next PATH object
    }PATH;

    typedef struct circgraph{                       //Struct to store the graph and the demand of the circulation problem
        graph * graphptr;
        int demand;
    }CIRCGRAPH;


class AdjacencyList{                                // Class declaration. This class is for accepting the values from the input file

public:


    GRAPH* readGraph(string filename);          //Member function declaration to read the graph from the text file and store as an adjacency list


};

class BFS{                                       // Class declaration. This class is for implementing the BFS algorithm

public:
        BFSLIST  * createBFSList(GRAPH * start, int source); //Member function to implement the BFS algorithm
        PATH * shortestPath(int destination, int source, BFSLIST * bfstlist, string choice); //member function to traverse through BFSTREE and get the shortest path and distance
};

class FFA{                                                                              //Class to implement Ford Fulkerson Algorithm

public:
    int fordFulkerson(GRAPH * start);                                                   //Starting function. Finds out the source and sink and calls other functions to find out the mas flow
    int implementFFA(GRAPH * start, int source, int sink);                              //calls the BFS and gets the shortest path from source to sink
    int findMinCapacity(GRAPH * start, PATH * shortpath);                               //Finds the minimum capacity in the given path
    void updateResidualNetwork(GRAPH * start, PATH * shortpath , int mincapacity);      //Updates the residual network with augmented path
};

 class Demand {                                                                         //Class to implement the circulation problem
     public:
        CIRCGRAPH * readDemandGraph(string filename);                                   //Reads the input. Sets all negative demands to supersource and positive demands to super sink
 };

GRAPH* AdjacencyList:: readGraph(string filename){                                 //Function definition

    string input;                                                                  //each line from the text file is read to the variable input
    int node,weight,i=0;                                                           // initializing the variables used
    ADJNODES *previousnode = NULL;
    NODES * topnode = NULL;
    GRAPH *graphobj = NULL;
    NODES * newnode = NULL;

        ifstream inputfile;                                                           //initialize fstream to read mode with ifstream
        inputfile.open(filename.c_str());                                             //open the text file entered by the user
        if(inputfile.is_open()){                                                       //checking whether the input file is open to avoid runtime exceptions
            graphobj = (GRAPH*) malloc(sizeof(GRAPH));                                 //Allocates memory used for the graph object and stores its value to the GRAPH variable
            while(getline(inputfile,input)){                                           //reads input file line by line
                newnode = (NODES*) malloc(sizeof(NODES));                              //Allocates memory used for the node object and stores its value to the NODE variable
                newnode -> nodevalue = i;                                              //Since the first line is the adjacent nodes of node 0, or the line number shows the node value, variable i is used to store the values
                newnode -> adjpointer = NULL;                                          //sets the pointers to NULL
                newnode ->down = NULL;
                if(topnode != NULL){                                                   //Checking whether this is not the first node
                    topnode ->down = newnode;                                          //current node address is assigned to the previous/top node pointer
                }
                graphobj ->graphnodes[i] = (NODES *) malloc(sizeof(NODES));
                graphobj ->graphnodes[i] = NULL;
                graphobj ->graphnodes[i++] = newnode;                                  //adds the new node to the array of nodes in the GRAPH object
                topnode = newnode;
                if(!inputfile.eof()){                                                   //reads the input till the end of file
                    stringstream inputvalues(input);                                    //separates the input values by space
                    while(inputvalues >> node >> weight){                               //assigns the input values to integer
                        ADJNODES *  newadj = (ADJNODES*) malloc(sizeof(ADJNODES));      //Allocates memory used for the graph object and stores its value to the GRAPH variable
                        newadj -> nodevalue = node;                                     //sets the node value(vertext) and weight(capacity of the edge)
                        newadj -> weight = weight;
                        newadj -> next = NULL;
                        if(newnode -> adjpointer == NULL){                              //If this is the first adjacent node, sets its address to NODE's adjacentpointer
                            newnode -> adjpointer = newadj;
                        }else{
                            previousnode -> next = newadj;                              //otherwise, sets this address to previous nodes next pointer
                        }
                    previousnode = newadj;
                    }
                }
            }
            inputfile.close();                                                      //closes the opened input file
         }else{
            throw 0;                       //if file not opened appropriate message is shown
         }

    if(graphobj != NULL){                                                       //Stores the total nodes/vertex if its not an empty file
        graphobj ->numberofnodes = i;
    }
    return graphobj;                                                            //returns the graph object. will return NULL for an empty file
}



BFSLIST * BFS :: createBFSList(GRAPH * start, int source){                           //function definition

    int totalnode =start ->numberofnodes;                               //total nodes used to make arrays to store status and distance. status "WHITE" is for not visited, GREY visited bur adjacent nodes not visited, BLACK visited all the adjacent nodes.
    string status [totalnode] ;                                         //array to store status
    int i,currentnode,visitingnode;
    queue <int> bfsque;                                                 // Queue to keep the neighboring nodes
    int distarray [totalnode];                                          //array to store the distance of each node from the start node or node0
    ADJNODES * adjnodes = NULL;                                         //initializes all pointers to NULL
    BFSLIST * prevnode = NULL;
    BFSLIST * newnode = NULL;
    BFSLIST * bfsnode = NULL;

    if(source < totalnode){                                                 //checks for invalid input

        for(i= 0; i<totalnode; i++){                                        //sets the status of each node to "WHITE" and distances to 0
            status [i] = "WHITE";
            distarray[i] = -1;
        }
        distarray[source] = 0;
        currentnode = start -> graphnodes[source] ->nodevalue;                //visits the starting node creates a BFSTREE and stores the value
        bfsnode = (BFSLIST*) malloc(sizeof(BFSLIST));                          //creates a node for source and stores corresponding values
        bfsnode ->nodevalue = currentnode;
        bfsnode ->parentnode = currentnode;
        bfsnode ->distance =  distarray[source];
        bfsnode ->next = NULL;
        prevnode = bfsnode;
        status [currentnode] = "GREY";                                              //sets the status to GREY
        bfsque.push(currentnode);                                                   //pushes current node to QUEUE

        while(! bfsque.empty()){                                                    //visits the neighboring nodes of the Queue front
            currentnode = bfsque.front();
            bfsque.pop();
            if(status [currentnode] != "BLACK"){
                adjnodes = start -> graphnodes[currentnode] ->adjpointer;
                    while(adjnodes != NULL){
                        visitingnode = adjnodes ->nodevalue;
                        if(status [visitingnode]  == "WHITE"){                     //adds the neighboring nodes and its parent and distance(adds 1 to the distance of its parent node)
                            newnode = (BFSLIST*) malloc(sizeof(BFSLIST));
                            newnode ->nodevalue = visitingnode;
                            newnode ->parentnode = currentnode;
                            newnode ->distance = distarray[currentnode]+1;
                            newnode ->next = NULL;
                            prevnode ->next = newnode;
                            prevnode = newnode;
                            bfsque.push(visitingnode);
                            status [visitingnode] = "GREY";
                            distarray[visitingnode]=distarray[currentnode] + 1;
                        }
                        adjnodes = adjnodes ->next;
                }
                status [currentnode] = "BLACK";
            }else if(currentnode == source){                                        //checks for loop
                break;
            }
        }
    }
    return bfsnode;

}

 PATH * BFS :: shortestPath(int destination, int source, BFSLIST * bfslist, string choice){     //function definition to get the shortest path from the output of the BFS algorithm

     PATH * previous = NULL;
     BFSLIST * bfsstart = bfslist;
     int distance = -1, sourcedist = 0;
     PATH * shortpath;
    do{
        if((destination == bfsstart ->nodevalue) ){
            break;
        }
        while (bfslist != NULL){
            if(bfslist ->nodevalue == destination){                             //traverse the list to find the destination node
                shortpath =(PATH*) malloc(sizeof(PATH));
                if(distance == -1){                                             //gets the distance of the given destination
                    distance = bfslist ->distance ;
                    previous = NULL;
                }
                shortpath ->node = bfslist ->nodevalue;                     //adds the path
                shortpath ->next = previous;
                previous= shortpath;
                destination = bfslist ->parentnode;                         //sets the parent of the destination node as the new destination
                bfslist=bfsstart;                                           //searches for the new destination beginning from the starting of the list
                break;
            }else{
                if(bfslist ->nodevalue == source){
                    sourcedist = bfslist ->distance;                        //stores the value of the source to get the distance from intermediate nodes
                }
                 bfslist = bfslist ->next;
            }
        }
    }while ((destination != source) && (bfslist != NULL));                  //gets the path till the inputed source
    if(distance == -1){
        shortpath = NULL;                                                   //sets null for no path
    }else{
        shortpath = (PATH*) malloc(sizeof(PATH));                          //adds the starting node to the list
        shortpath ->node = source;
        shortpath ->next = previous;
    }
    distance = distance-sourcedist ;                                        //gets the distance from nodes other than the starting node
    if(choice.compare("-b") == 0){
            cout <<endl<< "The distance is : " << distance <<endl;          //For BFS output is shown, shows -1 for no path
    }

    return shortpath;
 }

 int FFA :: fordFulkerson(GRAPH * start){                                       //function implementation
     AdjacencyList obj;
     PATH * shortpath;
     BFS bfsobj;
     NODES * nodeptr;
     ADJNODES * adjptr;
     int source,sink,i, maxflow = 0;

     int sourcefinder[start ->numberofnodes];                                      //array to find the source, sets all the values to 0

     for(i =0 ; i< start ->numberofnodes; i++){
        sourcefinder[i]=0;
     }
     nodeptr = start ->graphnodes[0];                                           //if a node is not present in adjnodes its source
     while(nodeptr != NULL){
        adjptr = nodeptr ->adjpointer;
        if(adjptr == NULL){
          sink = nodeptr ->nodevalue;
        }
        while(adjptr != NULL){                                                  //if the adj node is NULL its sink
            sourcefinder[adjptr ->nodevalue] = -1;
           adjptr = adjptr ->next;
        }
        nodeptr = nodeptr ->down;
    }
    for(i = 0; i < start ->numberofnodes; i++){                                 //checking the missing node in adjacency list
        if(sourcefinder[i] == 0){
            source = i;
            break;
        }
    }
maxflow = implementFFA( start, source, sink);
return maxflow;
 }
 /*This program avoids the initial setting of flows to zero. Every time, the capacity is reduced by the flow and the back flow is updated with new flow(adds to old flow if there is any)*/
 int FFA:: implementFFA(GRAPH * start, int source, int sink){
     PATH * shortpath, * shortpathcopy;
     BFSLIST * bfslist;
     NODES * nodeptr;
     ADJNODES * adjptr, * previous = NULL;
     BFS bfsobj;
     int mincapacity = 0;
     int maxflow = 0;

    do{
         bfslist = bfsobj.createBFSList(start,source);                                  //calls bfs
         if(bfslist == NULL){
             break;
         }
         shortpath = bfsobj.shortestPath(sink, source, bfslist, "-f");                     //gets shortest path
         shortpathcopy = shortpath;
         if (shortpath == NULL){
            return maxflow;
         }
        mincapacity = findMinCapacity(start,shortpath);                                 //finds the minimum capacity
        maxflow += mincapacity;                                                           //ads to the max flow
        updateResidualNetwork(start, shortpath, mincapacity);
     }while(shortpath != NULL);
     return maxflow;
 }

 int FFA :: findMinCapacity(GRAPH * start, PATH * shortpath){
     NODES * nodeptr;
     ADJNODES * adjptr;
     int mincapacity =0;

        while(shortpath ->next != NULL){                                                      //traverse the PATH and finds min capacity
             nodeptr = start ->graphnodes[shortpath ->node];
             adjptr = nodeptr ->adjpointer;
             shortpath = shortpath ->next;

             while(adjptr != NULL){
                if(adjptr->nodevalue == shortpath ->node){
                        if(mincapacity == 0){                                                 //for the first pass, sets the minimum capacity to weight of the node
                            mincapacity =adjptr->weight;
                        }
                        if(adjptr->weight < mincapacity){                                   //checks for minimum capacity
                            mincapacity = adjptr ->weight;

                        }
                        adjptr= adjptr ->next;
                            break;
                        }
                adjptr= adjptr ->next;
             }
     }
     return mincapacity;

 }


 void FFA :: updateResidualNetwork(GRAPH * start, PATH * shortpath , int mincapacity){                          //function to update the residual network

    NODES * nodeptr = NULL, *backnodeptr = NULL;
    ADJNODES * adjptr = NULL, * previous = NULL, * backprevious = NULL, * backadjptr = NULL;
    int flag = 0;                                                                                               //to check whether the back flow already present or not

      while((shortpath ->next != NULL) ){
            nodeptr = start ->graphnodes[shortpath ->node];
            adjptr = nodeptr ->adjpointer;
            shortpath = shortpath ->next;
            previous = NULL;
              while(adjptr != NULL){                                                                        //finds out the edge in the path
                if(adjptr->nodevalue == shortpath ->node){
                    if(adjptr->weight - mincapacity == 0){                                                  //removes the edge
                        if(previous == NULL){
                             nodeptr -> adjpointer = adjptr ->next;
                        }else{
                            previous ->next = adjptr ->next;
                        }
                    }else{
                        adjptr->weight = adjptr->weight - mincapacity;                                      //updates the weight
                    }
                    backnodeptr = start ->graphnodes[shortpath ->node];                                     //checks for back flow
                    backadjptr = backnodeptr ->adjpointer;
                    backprevious = NULL;
                    while(backadjptr != NULL){
                         if(nodeptr ->nodevalue == backadjptr->nodevalue){
                                backadjptr->weight = backadjptr->weight + mincapacity;                     //adds the flow to weight if already present
                                flag = 1;
                                break;

                    }
                    backprevious = backadjptr;
                    backadjptr = backadjptr -> next;
                    }
                    if(flag==0){                                                                        //creates a new edge with weight is same as the flow
                         ADJNODES *  newadj = (ADJNODES*) malloc(sizeof(ADJNODES));
                         newadj -> nodevalue = nodeptr ->nodevalue;
                         newadj ->weight = mincapacity;
                         newadj ->next = NULL;
                         if(backprevious == NULL){
                            backnodeptr ->adjpointer = newadj;
                         }else{
                             backprevious -> next =newadj;
                         }
                }

                    break;
                }
                previous = adjptr;
                adjptr = adjptr ->next;

         }
    }
 }

CIRCGRAPH * Demand :: readDemandGraph(string filename){
      string input;                                                                  //each line from the text file is read to the variable input
    int node,weight,i=0,demand =0, totalsupply=0, totaldemand=0;                    // initializing the variables used, i is used to find out the sink
    ADJNODES *previousnode = NULL;
    NODES * topnode = NULL;
    GRAPH *graphobj = NULL;
    NODES * newnode = NULL;
    NODES * supersource = NULL;
    NODES * supersink = NULL;
    ADJNODES * supersinkadj = NULL;

    vector <pair<NODES * , int> > adjsink;                                      //to store the nodes with positive demands and at the end assigns to super node

    ifstream inputfile;                                                           //initialize fstream to read mode with ifstream
    inputfile.open(filename.c_str());                                             //open the text file entered by the user
    if(inputfile.is_open()){                                                       //checking whether the input file is open to avoid runtime exceptions
        graphobj = (GRAPH*) malloc(sizeof(GRAPH));                                 //Allocates memory used for the graph object and stores its value to the GRAPH variable
        supersource = (NODES*) malloc(sizeof(NODES));
        supersource ->nodevalue = i;
        supersource ->down = NULL;
        supersource->adjpointer = NULL;
        graphobj ->graphnodes[i] = (NODES *) malloc(sizeof(NODES));
        graphobj ->graphnodes[i] = NULL;
        graphobj ->graphnodes[i++] = supersource;
        while(getline(inputfile,input)){                                           //reads input file line by line
            newnode = (NODES*) malloc(sizeof(NODES));                              //Allocates memory used for the node object and stores its value to the NODE variable
            newnode -> nodevalue = i;                                              //Since the first line is the adjacent nodes of node 0, or the line number shows the node value, variable i is used to store the values
            newnode -> adjpointer = NULL;                                          //sets the pointers to NULL
            newnode ->down = NULL;
            previousnode = NULL;
            if(topnode != NULL){                                                   //Checking whether this is not the first node
                topnode ->down = newnode;                                          //current node address is assigned to the previous/top node pointer
            }else{
                supersource ->down = newnode;
            }
             graphobj ->graphnodes[i] = (NODES *) malloc(sizeof(NODES));
            graphobj ->graphnodes[i] = NULL;
            graphobj ->graphnodes[i++] = newnode;                                  //adds the new node to the array of nodes in the GRAPH object
            topnode = newnode;
            if(!inputfile.eof()){                                                   //reads the input till the end of file
                stringstream inputvalues(input);                                    //separates the input values by space
                inputvalues >> demand;
                if(demand < 0){
                    totalsupply = totalsupply + demand;
                    ADJNODES *  newadj = (ADJNODES*) malloc(sizeof(ADJNODES));      //Allocates memory used for the graph object and stores its value to the GRAPH variable
                    newadj -> nodevalue = i-1;                                     //sets the node value(vertext) and weight(capacity of the edge)
                    newadj -> next = NULL;
                    newadj -> weight = demand * -1;
                    newadj -> next = supersource -> adjpointer ;
                    supersource -> adjpointer = newadj;

                }else{                                                              //saves for super sink
                    totaldemand = totaldemand + demand;
                    adjsink.push_back(make_pair(newnode,demand));
                }
                while(inputvalues >> node >> weight){                               //assigns the input values to integer
                    ADJNODES *  newadj = (ADJNODES*) malloc(sizeof(ADJNODES));      //Allocates memory used for the graph object and stores its value to the GRAPH variable
                    newadj -> nodevalue = node+1;                                     //sets the node value(vertext) and weight(capacity of the edge)
                    newadj -> weight = weight;
                    newadj -> next = NULL;
                    if(newnode -> adjpointer == NULL){                              //If this is the first adjacent node, sets its address to NODE's adjacentpointer
                        newnode -> adjpointer = newadj;
                    }else{
                        previousnode -> next = newadj;                              //otherwise, sets this address to previous nodes next pointer
                    }
                previousnode = newadj;
                }
            }
        }
        supersink = (NODES*) malloc(sizeof(NODES));                                //creates node for super sink
        supersink ->nodevalue = i;
        supersink ->adjpointer = NULL;
        supersink ->down = NULL;
        topnode ->down = supersink;

        for(int j = 0; j < adjsink.size(); j++){
            ADJNODES *  newadj = (ADJNODES*) malloc(sizeof(ADJNODES));      //Allocates memory used for adjacent node
            newadj ->next = NULL;
            newadj ->nodevalue = i;                                         //sets the sink value
            newadj ->weight = adjsink[j].second;
            newnode = adjsink[j].first;
            if(newnode != NULL){
                if(newnode ->adjpointer == NULL){
                    newnode ->adjpointer = newadj;
                }else{
                    previousnode = newnode ->adjpointer;
                    while(previousnode ->next != NULL){
                        previousnode = previousnode ->next;
                    }
                     previousnode ->next = newadj;
                }
            }
        }

        graphobj ->graphnodes[i] = (NODES *) malloc(sizeof(NODES));        //adds the sink to the graph
        graphobj ->graphnodes[i] = NULL;
        graphobj ->graphnodes[i++] = supersink;
        inputfile.close();                                                      //closes the opened input file
         }else{
            throw 0;                       //if file not opened appropriate message is shown
         }

    if(graphobj != NULL){                                                       //Stores the total nodes/vertex if its not an empty file
        graphobj ->numberofnodes = i;
    }
    CIRCGRAPH * circgraphobj = (CIRCGRAPH*) malloc(sizeof(CIRCGRAPH));      //stores the graph and demand
    circgraphobj ->demand = totaldemand;
    circgraphobj  ->graphptr = graphobj;
    if(totaldemand+totalsupply != 0){                                       //demand and supply not matches, no circulation
        circgraphobj  ->graphptr = NULL;
        cout <<endl<< "No. Its has no circulation ! Total supply is  " << (totalsupply * -1) <<"  and Total demand is  " << totaldemand <<endl;
    }
    return circgraphobj;
}
int main(){                                                                             //main function. The program starts here

    AdjacencyList obj;
    GRAPH * start;
    BFSLIST * bfslist;
    BFS bfsobj;
    FFA ffaobj;
    PATH * shortpath;
    Demand demandobj;
    CIRCGRAPH * circgraphobj;
    int maxflow = 0;
    string input, filename, choice;
    int source =-1, sink =-1;
    char sel = 'n';                                                                         // char to check whether the user wants to continue
    try{
        getline( cin, input );                                                              //reads the input from the console
        stringstream inputvalues(input);
        inputvalues >> choice >> filename ;
        inputvalues >>  source >> sink;
        if(choice.compare("-b") == 0){
            if((source != -1) && (sink != -1)){                                                   //calls function depending on the input
                start = obj.readGraph(filename);
                bfslist = bfsobj.createBFSList(start,source);
                if(bfslist != NULL){
                    shortpath = bfsobj.shortestPath(sink, source, bfslist,choice);
                    if(shortpath != NULL){
                        cout << endl <<"The path is";
                        while(shortpath != NULL){                                                      //traverse the PATH and prints the output
                            cout  <<", " <<shortpath ->node ;
                            shortpath =shortpath ->next;
                        }
                    }
                    cout <<endl;
                }
                else{
                    cout << "Check your input";
                }
            }else{
                cout << "Check your input";
            }
        }else if(choice.compare("-f") == 0){                                                        //calls function to find maximum flow
            start = obj.readGraph(filename);
            maxflow = ffaobj.fordFulkerson(start);
            cout <<endl<< "Maximum flow is : " << maxflow <<endl;
        }else if(choice.compare("-c") == 0){                                                        //calls the function for circulation problem
            circgraphobj = demandobj.readDemandGraph(filename);
            if(circgraphobj  ->graphptr != NULL){
                start = circgraphobj->graphptr;
                maxflow = ffaobj.fordFulkerson(start);
               if(maxflow == circgraphobj->demand ){
                    cout <<endl<< "Yes. Its has a circulation !" <<endl;
               }
               else{
                cout <<endl<< "No. Its has no circulation ! Total demand is  " << circgraphobj->demand  <<"  and Maximum flow is  " << maxflow <<endl;
              }
           }
        }else
            cout <<endl<< "Wrong input";
    }catch(int message){                                                                                        //catches file exception
        cout << endl << "Exception caught and handled. Check your file name";
        }

}
