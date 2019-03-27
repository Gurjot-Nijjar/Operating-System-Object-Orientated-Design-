/*
NAME : GURJOT SINGH NIJJAR
INSTRUCTOR : PAVEL SHOSTAK
COURSE : CSCI 34000 OPERATING SYSTEMS
PROGRAM DESCRIPTION : You should write a program that simulates some aspects of operating systems. There is no real system programming involved. 
The whole simulation is based on the text inputs that the program receives from user.
 
Scheduling: your program should use multilevel feedback queue scheduling. Top level 0 queue uses RR scheduling with a single time quantum. Level 1 queue uses RR scheduling with a 
double time quantum.  Lowest level 2 queue uses FCFS approach. Each process starts at the level 0 queue. Each time the process exceeds its time limit (in other words, each time 
the process is preempted by the q command) it goes one level lower. We do not implement any mechanics to move processes up the levels.
The process arriving to a higher-level queue preempts running process from the lower-level queue. The process preempted by the higher-level process returns to the head of its queue. 
The process from the lower-level queue can run only if all higher queues are empty.
 
All I/O-queues are FCFS.
 
Memory: your program should simulate demand paging memory management. When a new process appears in the system, its page #0 is loaded into memory. Later pages are 
added only when needed (when corresponding m command is used). If page needs to be loaded into memory but there is no place for it, swap from the RAM the least recently used page 
(it can belong to any process). 
*/

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <deque>

using namespace std;

/*
A process structure made in order for us to store specific functions and specific identification methods
each process has a unique PID, a file associated with the PID, and the time quantums spent in the CPU for each process
*/

struct Process
{
    public:

    void set_PID(int pid)
    {
        PID = pid;
    }

    void time_Quantum_Counter()
    {
        time_quantum++;
    }

    void set_TQ(int time)
    {
        time_quantum = time;
    }

    void set_File(string file)
    {
        input = file;
    }

    string get_File()
    {
        return input;
    }

    int get_PID()
    {
        return PID;
    }

    int set_page(int p_Number)
    {
        page_Number = p_Number;
    }

    int get_page()
    {
        return page_Number;
    }

    int get_time_Quantum_spent()
    {
        return time_quantum;
    } 

    void operator = (const Process &other)
    {
        PID = other.PID;
        time_quantum = other.time_quantum;
    }

    private:
    string input;
    int PID = 0;
    int time_quantum = 0;
    int page_Number = 0;
};

/*
PCB_CPU is the main CPU structure to hold the current process running in the CPU, it is necessaery in order for the simulation to make sure what process is currently running and not
in the ready queue, from the cpu structure you may contain the PID, the file using CPU and the process to set and file to set to use the pcb cpu
*/
struct PCB_CPU
{

   public:
    //iniatilizing a PCB structure with a defined PID 
    

    void set_Process(int PID)
    {
        _PID  = PID;
    }

    int get_PID()
    {
        return _PID;
    }

    void set_File(string init_File)
    {
        _file = init_File;
    }

    string get_File()
    {
        return _file;
    }  

    bool isEmpty()
    {
        if(_PID == 0)
        {
            return true;
        }
    }
    private:
    // keep it private so main cant access or edit let PCB control as it should
     int _PID = 0;
     string _file;
     
};
/*
Our ready queue is where all the magic happens and where it is possible to maintain the actions required for our simulation. Ready queue uses the multilevel feedback queue scheduling
algorthm in order for us to properly implement, three deques of the process structure was iniated to hold the processes inside the queue, basically three double ended queues for one
big queue. I mantain this ready in order to print out the ready queue by Level, and set Pages number and time quantums for the ready queue to push a process to PCB_CPU
and functions to move processes alongside the queues

*/

struct ready_Queue
{
    public:

    void queue_Process(Process &process)
    {
        Level_0_RR.push_back(process);
    
    }

   Process getRunningProcess()
    {  
        if (Level_0_RR.empty() != true)
        {
            return Level_0_RR.front();
        }

        else if (Level_1_RR.empty() != true)
        {
            return Level_1_RR.front();
        }

        else if (Level_2_FCFS.empty() != true)
        {
           return Level_2_FCFS.front();
        }
    }

    bool set_Page_N(int n)
    {
         
        if (Level_0_RR.empty() != true)
        {
            Level_0_RR.front().set_page( n);
            return true;
        }

        else if (Level_1_RR.empty() != true)
        {
            Level_1_RR.front().set_page( n);
            return true;
        }

        else if (Level_2_FCFS.empty() != true)
        {
            Level_2_FCFS.front().set_page( n);
            return true;
        }
    
    }

    bool remove_Process()
    {

        if (Level_0_RR.empty() != true)
        {
             Level_0_RR.pop_front();
             return true;
        }

        else if (Level_1_RR.empty() != true)
        {
              Level_1_RR.pop_front();
              return true;
        }

        else if (Level_2_FCFS.empty() != true)
        {
            Level_2_FCFS.pop_front();
            return true;
        }

    }


    void time_Quantum_Counter()
    {
        time_Quantum++;

        if (Level_0_RR.empty() != true)
        {
            Level_0_RR.front().time_Quantum_Counter();
        }

        else if (Level_1_RR.empty() != true)
        {
            Level_1_RR.front().time_Quantum_Counter();
        }

        else if (Level_2_FCFS.empty() != true)
        {
             Level_2_FCFS.front().time_Quantum_Counter();
        }
    }

    int get_time_Quantum_spent()
    {
        return time_Quantum;
    }

    void reset_Time_Quantum()
    {
        time_Quantum = 0;
    }

    void Level_0_to_Level_1()
    {                                                                                                       
        if (Level_0_RR.empty() != true)
        {
            Level_1_RR.push_back(Level_0_RR.front());
            Level_0_RR.pop_front();
        }
    }

    void Level_1_to_Level_2()
    {
        if (Level_1_RR.empty() != true)
        {
            Level_2_FCFS.push_back(Level_1_RR.front());
            Level_1_RR.pop_front();
        }
    }
    bool Level_2_Empty()
    {
         return ( Level_2_FCFS.empty() == true);
    }

    bool Level_1_Empty()
    {
        return (Level_1_RR.empty() == true);
    }

    bool Level_0_Empty()
    {
        return (Level_0_RR.empty() == true);
    }

    void print()
    {
        if(Level_0_RR.empty() != true)
        {
            deque<Process> copy_Level0 = Level_0_RR;
            copy_Level0.pop_front();
            while(!copy_Level0.empty())
            {
                cout << "Process with PID: " << copy_Level0.front().get_PID() << " in Level 0 is in the ready queue." << endl;
                copy_Level0.pop_front();
            }
        }

        if(Level_1_RR.empty() != true)
        {
            deque<Process> copy_Level1 = Level_1_RR;
            if (Level_0_RR.empty() == true)
            {
                copy_Level1.pop_front();

            }

            while(!copy_Level1.empty())
            {
                cout << "Process with PID: " << copy_Level1.front().get_PID()  << " in Level 1 is in the ready queue." << endl;
                copy_Level1.pop_front();
            }
        }

        if(Level_2_FCFS.empty() != true)
        {
            deque<Process> copy_Level2 = Level_2_FCFS;
             if (Level_0_RR.empty() == true && Level_1_RR.empty() == true)
            {
                copy_Level2.pop_front();

            }
            while(!copy_Level2.empty())
            {
                cout << "Process with PID: " << copy_Level2.front().get_PID()  << " in Level 2 is in the ready queue." << endl;
                copy_Level2.pop_front();
            }
        }
        
    }

    private:
    
    deque<Process> Level_0_RR;
    deque<Process> Level_1_RR;
    deque<Process> Level_2_FCFS;
    int time_Quantum = 0;


};

/*

our input output queue mantains the hard disks as asked by our user and the queue is used with a deque of processes and the implemented in our main with another deque of io queues
each individual for the iniation of the many hard disks and all the possibilities

*/

struct IO_Queue
{
    public:

    Process In_Use()
    {
        return p_HDD.front();
    }

    void queue_Process(Process &process)
    {
        p_HDD.push_back(process);
    }

    void exit_Process()
    {
        p_HDD.pop_front();
    }

    bool empty()
    {
        return (p_HDD.empty());
    }

    void print()
    {
        deque<Process> copy = p_HDD;
        while(!copy.empty())
        {
            cout << "Process with PID: " << copy.front().get_PID() <<  " is in Hard Disk Device queue. ";
            copy.pop_front();
        }
    }


    private:

    deque<Process> p_HDD;
};

/*
This structure is used for our main memory management and in order for us to update the memory Memory commands are configured where we also store the Time Quantums of the processes 
and etc

*/

struct Page_Table
{
    public:

    Page_Table(int size)
    {
        _size = size;
        LRU_time_quantum = 1;
        frames_Used = 1;
        initialize_Frames();
    }

    bool add_Page(Process process)
    {
        
            for(int x = 0; x < _size; x++)
            {
                if(page_frame[x][1] == process.get_PID())
                {
                page_frame[x][2] = process.get_page();
                page_frame[x][0] = process.get_time_Quantum_spent();
                return true;
                }
            }
        
        
        if(frames_Used > _size)
        {
            int LRU_time_quantum = LRU_Frame();
            page_frame[LRU_time_quantum][0] = process.get_time_Quantum_spent();
            page_frame[LRU_time_quantum][1] = process.get_PID();
            page_frame[LRU_time_quantum][2] = process.get_page();
            return true;
        }

        else
        {
            for (int x = 0; x < _size;x++)
            {
                if(page_frame[x][1] == 0)
                {
                    page_frame[x][0] = process.get_time_Quantum_spent();
                    page_frame[x][1] = process.get_PID();
                    page_frame[x][2] = process.get_page();
                    frames_Used++;
                    return true; 
                }
            }
        }
    }

    void Print()
    {
         cout << "Frame Number | Time Quantums Spent |   PID   |  Page Number" << endl;
        for(int x = 0; x < _size; x++)
        {
         cout << "      " << x  << "      |          " << page_frame[x][0] << "          |    " << page_frame[x][1] << "    |      " << page_frame[x][2] << endl;
        }
    }

    void Remove(int _PID)
    {
        for (int x = 0; x < _size; x++)
        {
            if(page_frame[x][1] == _PID)
            {
                page_frame[x][0] = 0;
                page_frame[x][1] = 0;
                page_frame[x][2] = 0;
                
            }
        }
    }

    private:

    int LRU_time_quantum;
    int frames_Used;
    int _size;
    deque< deque<int> > page_frame;
    

    void initialize_Frames()
    {   
        page_frame.resize(_size);
        // make sure the page table is set to a certain size to accodomate the size given by the user when inputted into the simulation
        for (int x = 0; x < _size; x++)
        {
            page_frame[x].push_back(0);
            page_frame[x].push_back(0);
            page_frame[x].push_back(0);
        } // each frame will have a page number, its pid, and the time quantum spent, intializing the push statements three times to make sure each deque has three values 
          // for example, the deque page_frame[0], will have three values all zero to keep it empty( and not accessed but could be accessed by the cpu at a later time.)
    }

    int LRU_Frame()
    {
        int LRU;
        int time_quantum = page_frame[0][0];

        for (int x = 0; x < _size; x++)
        {
            if(page_frame[x][0] <= time_quantum)
            {
                LRU = x;
                time_quantum = page_frame[x][0];
            }
        }

        LRU_time_quantum = time_quantum;

        return LRU;

    }

};


int main()
{
    int ram_Size_bytes;
    int page_Size;
    int disk_Size;

    PCB_CPU PCB_CPU;
    PCB_CPU.set_Process(0);
    ready_Queue ready_Queue;
    //using the Ready queue structure to define our ready queue for our simulation and maintain the processes entering our simulation CPU
    deque<IO_Queue> HDD_queue;
    //using the IO queue structure to maintain the prompts hard disk in the simulated CPU 
    //Prompt the user the three questions in the beginning of the assignment.
    cout << "Welcome to the Operating System Simulator by Gurjot Singh Nijjar! For Professor Pavel Shostak :D" << endl;
    cout << " " << endl;
    cout << "How much RAM memory is there on the simulated computer? You can enter any number up to 4000000000 (4 billion)." << endl;
    cin >> ram_Size_bytes;
    cout << "What is the size of a page/frame?" << endl;
    cin >> page_Size;
    cout << "How many hard disks does the simulated computer have?" << endl;
    cin >> disk_Size;

    int frames;
    frames = ram_Size_bytes/page_Size;

    for (int x = 0; x < disk_Size; x++)
    {
        IO_Queue _HDD;
        HDD_queue.push_back(_HDD);
    }

    Page_Table page_Table(frames);
    int PID = 1;
    string commands;

    cout << "Only Commands with A, Q, t, d # file_name, D #, m address, S r , S i, S m will work. Thank you." << endl;

    while (getline(cin, commands))
    {
        if(commands[0] == 'A')
        {
             Process new_process;
             new_process.set_PID(PID);
             new_process.set_page(0);
             ready_Queue.queue_Process(new_process);
             if (PCB_CPU.isEmpty() == true)
             {
                 int running_pid = ready_Queue.getRunningProcess().get_PID();
                 PCB_CPU.set_Process(running_pid);
             }
             page_Table.add_Page(new_process);
             PID = PID + 32;
             cout << "A new process was created :)" << endl;
        }

        else if(commands[0] == 'Q')
        {
            cout << "Running process has spent 1 Time Quantum;" << endl;
            ready_Queue.time_Quantum_Counter();
            page_Table.add_Page(ready_Queue.getRunningProcess());
            if(ready_Queue.Level_0_Empty() != true)
            {
                if(ready_Queue.get_time_Quantum_spent() == 1)
                {
                    ready_Queue.Level_0_to_Level_1();
                    ready_Queue.reset_Time_Quantum();
                    PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());


                }
            }
            else if(ready_Queue.Level_1_Empty() != true )
            {
                if(ready_Queue.get_time_Quantum_spent() == 2)
                {
                    ready_Queue.Level_1_to_Level_2();
                    ready_Queue.reset_Time_Quantum();
                    PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());

                }
            }
            else
            {
                PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());
            }
        
            
        }

        else if(commands[0] == 't')
        {
            cout << "Running process has terminated" << endl;
            page_Table.Remove(ready_Queue.getRunningProcess().get_PID());
                if(ready_Queue.remove_Process() == true)
                {
                    PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());
                }
                if(ready_Queue.Level_0_Empty() && ready_Queue.Level_1_Empty() && ready_Queue.Level_2_Empty())
                {
                    PCB_CPU.set_Process(0);
                }
        }

        else if(commands[0] == 'd')
        {
            string number_ = commands.substr(2,2);
            int number;
            stringstream stringtoint(number_);
            stringtoint >> number;
            string input_file = commands.substr(4, commands.length());
            PCB_CPU.set_File(input_file);
            Process process;
            process.set_PID(PCB_CPU.get_PID());
            process.set_File(input_file);
            HDD_queue[number].queue_Process(process);
            if( ready_Queue.remove_Process() == true)
            {
                PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());
            }
            else
            {
            PCB_CPU.set_Process(0);
            }
            cout << "process has requested device number: " << number << " to read or write the file" << input_file << endl;

        }

        else if(commands[0] == 'D')
        {
            if(HDD_queue.size() > 0)
            {
                string number = commands.substr(2,2);
                int number_;
                stringstream stringtoint(number);
                stringtoint >> number_;
                if(HDD_queue[number_].empty() == false)
                {
                    Process process;
                    process = HDD_queue[number_].In_Use();
                    ready_Queue.queue_Process(process);
                }
                HDD_queue[number_].exit_Process();
                PCB_CPU.set_Process(ready_Queue.getRunningProcess().get_PID());
                cout << "Hard Disk " << number_ << " has finshed the work" << endl;
            }

        }

        else if(commands[0] == 'm')
        {
            string number = commands.substr(2,commands.length());
            int number_;
            stringstream stringtoint(number);
            stringtoint >> number_;
            int pag__number = number_ / page_Size;
            ready_Queue.set_Page_N(pag__number);
            page_Table.add_Page(ready_Queue.getRunningProcess());
            cout << "Process requests a memory operation" << endl;
        }

        else if(commands[0] == 'S')
        {
            if(commands[2] == 'r')
            {
                cout << "Process with PID number " <<  PCB_CPU.get_PID() << " is using the CPU, if 0 then there is no process using the CPU." << endl;
                cout << "These are the processes that are waiting in the ready queue. " << endl;
                ready_Queue.print();
            }

            else if(commands[2] == 'i')
            {
                for (int x = 0; x < disk_Size; x++)
                {
                    if(HDD_queue[x].empty() == false)
                    {
                        cout <<"Device Number: " << x << ",  with PID Number: " << HDD_queue[x].In_Use().get_PID() << " and File Number: "  << HDD_queue[x].In_Use().get_File() << " are using disks. " << endl;
                    }
                }
            }

            else if(commands[2] == 'm')
            {
                page_Table.Print();
            }

            else
            {

            }
        }

        else
        {

        }




    }
    
    
}

