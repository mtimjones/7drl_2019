
# 7DRL / 2019 ("Rogue Hack")

Roguelike in a cyberpunk computer network setting. You travel through 
nodes in a network to reach the exit, using exploits to defeat rogue 
processes intent on disconnecting you. Your process (login) represents 
your virtual entity in the network, protect it and destroy those processes 
who would do it harm to advance through the network.

A simple network represents a level. The network is made up of a number 
of randomly connected nodes that may be explored. An entry node permits 
traveling to the next network (level).

The entire dungeon is made up of a linear set of networks which are all 
randomly created with random connections and processes).

# Background

"It was a decade since you last jacked in, but this was old hardware and 
the perfect job for your aging expertise. This is your last opportunity 
to clear your name and escape the shadows.  Your goal is to dive deep 
into Sense/Net's corporate network to defeat the executive process and 
bring the corporation down. Type ‘help’ for more information."

# Goal

Reach the final node in the network to defeat the executive process.

Intro provided to set the scenario with an introduction in the beginning 
of a process that has minimal def and 0 att. This will provide a way to 
introduce the player to the commands and how to use them.

# Conflict System

Two forms of conflict exist in the game; traditional combat and hacking. 
Each has their own form of reward.  Combat system will be both simple and 
deterministic. Each process has stats that indicate the health, attack and 
defense capabilities. A simple equation will determine the hit probability:

```
p( hit ) = ( att / ( att + def ) )
```

The damage is defined by the exploit being used (having a certain amount 
of random damage possibility, such as 1-3 damage potential).  Once a 
process is defeated via combat, experience points are gained.  Hacking 
allows the user to defeat processes in a single action. The action is a 
keystroke sequence that must be performed within a certain amount of time. 
The higher the difficulty of the hack, the more keystrokes are required of 
the player to attack.  Once a process is hacked, it disappears and the 
player is rewarded energy as a function of the processes energy. If the 
player fails the hack, the process must be defeated through traditional
combat.

# Progression System

A standard progression system exists in the game where experience points 
accumulate to allow a player to increase level and a drop is rewarded 
with each level increase.  A drop consists of a new learned exploit. 
Exploits are learned randomly but with some order (based upon their level). 
See the level column in the commands table.

# Commands and Exploits

There exist a set of standard commands available to the user (such as 
connect, exit, wait, bash) and then a set of exploit commands that are 
learned through drops. The learned commands are learned at random to 
support replay ability.  The user may not leave a node until all enemy 
processes are killed or paused. A suspended process will allow the user 
to exit if needed (but the user gets no reward for clearing the node).

```
      Command    Type      Damage     Notes                      Level
      Help       Standard  N/A        Lists the available            0
                                      commands to the user.
      Connect    Standard  N/A        Connect to a new node          0
                                      within the network.
      Exit       Standard  N/A        Exit from the current          0
                                      node and return to the
                                      prior connected node.
      Wait       Standard  N/A        Skip a turn (may not be        0
                                      needed, useful with AP
                                      style).
      Bash       Standard  1-3        Initial attack exploit,        0
                                      minimal damage.
      Hack       Standard  N/A        Hack and then drain the        0
                                      energy from the process
                                      (no random drop). Hack
                                      requires a timed key
                                      sequence to unlock.

      Cut        Learned   3-5        Learned attack exploit,        1
                                      medium damage.
      Suspend    Learned   N/A        Temporarily suspends an        2
                                      enemy process for 2 turns.
      Wall       Learned   2-4        Learned buff to increase       3
                                      user defense for 3 turns.
      Kill       Learned   4-8        Learned attack exploit,        4
                                      larger damage.
      Trap       Learned   N/A        Temporarily suspends an        5
                                      enemy process for 4 turns.
      Barrier    Learned   N/A        Temporarily suspends all       6
                                      enemy processes for 2 turns.
      Counter    Learned   N/A        Return damage to the enemy     7
                                      process for next turn.
      Slice      Learned   4-8 / 2-4  Damage to and around process.  8
      **buff     Learned              Double the player damage in    9
                                      the next round, consumes this
                                      round.

* Help, Connect, and Exit do not count as a turn.
```


# Process Types

Encounters occur via processes on nodes. Each process has a purpose and a 
style of attack (sometimes no attack at all, but provides buffing or other 
feature). The user can invoke exploits that support the login process (such 
as armor).

** need to identify variety of behaviors for processes and bosses in the network nodes.

```
      Process   Description                     Attr   Rate Attack / Defense  Health
      login     This process represents the     @      100%                       15
                user’s login and the goal is
                to protect this process from 
                enemy processes.
      sentry    Basic armor process, provides    B D   100%      0 / 2             3
                an armor buff to other enemy
                processes.
      daemon    Basic enemy process, low armor    A     50%      2 / 4         3*lvl
                and damage.
      alarm
      encrypt   Obscures the PID of processes       S  100%      0 / 5         5*lvl
                until killed.
      watchdog  Calls in reinforcement minions.     S   50%      0 / 6            10
      upgrade   Basic damage buff for all        BA     50%      3 / 1            10
                enemy processes.
      fork      Replicating process that            S   50%      0 / 6
                generates new low-level 
                enemies over time.
      random    Changes to a random process         S   50%
                after first round.
      minion    Generated at some rate            A    100%      2 / 6             
                defined by level of child.
      executor  Level N boss, has the ability       S  100%      X / Y
                to spawn buffs to protect 
                itself.
      executive Final boss process, spawns a        S  100%      X / Y
                copy of itself before death.
```


# Process Attributes

The process attributes define the necessary information for a process. 
Some is displayed on the process list, some is hidden from the user and 
used internally.

```
      Attribute  Description                        Exposed/Hidden
      Type       The type of process attribute.     Exposed
      Energy     The amount of health for the       Exposed
                 process.
      Attack     The attack value for the process.  Exposed
      Defense    The defense value for the process. Exposed
      PID        The process identifier (randomly   Exposed
                 generated, but used when
                 specifying a command).
      Level      Level of the process               Exposed
      Function   The function pointer for the       Hidden
                 process (used to perform its
                 behavior on its turn).
      Rate       The rate at which this process     Hidden
                 can execute (1 for every turn, 2
                 for every other turn, etc.).
      State      State of the process (active,      Hidden
                 dead, suspended, etc.).
      Hackable   Boolean defines whether this       Exposed (attr)
                 process can be hacked.
```

# Major Structures

The node structure defines a node and its connectivity to other nodes. A 
level of nodes exists only while the player is in that level (otherwise, 
they are freed and the player cannot return).

```
typedef struct node_t
{
   char *ip_adrs;
   node_t connections[ 4 ]; // North, South, East, West.
   process_t *processes;
   bool visited;
   function_t *node_entry_function;
   function_t *node_exit_function;
} node_t;
```

The stats structure defines the common statistics used by the player and 
all enemies.

```
typedef struct stats_t
{
   uint8 attack;
   uint8 defense;
   uint8 energy;
} stats_t;
```

The attributes structure defines the attributes for a given process.

```
typedef struct attributes_t
{
   bool hackable; // H
   bool active;   // N/A
   bool user;     // U
   bool attack;   // A
   bool buff;     // B
   bool special;  // S, use process function for behavior.
} attributes_t;
```

The player and enemies are all represented by process_t.

```
typedef struct process_t
{
   process_type_t process_type;
   process_func_t *function;
   unsigned short pid;
   char *name;
   stats_t stats;
   attributes_t attributes;
   process_state_t state;
   uint8 state_value; // Value for current state (suspend, etc.).
   uint8 action_rate; // 1 = every turn.
} process_t;
```

# Game Loop

Given the turn-based strategy, the loop is greatly simplified compared to 
a real-time approach.  The loop expects input from the user and will then 
execute the request and then determine whether to execute a node-based 
action (for processes running on the node).  If the user invoked a command 
that involves a request, then each of the processes on the node are 
checked to see if they can invoke an action (based upon their rate). If 
not, the rate variable is decremented to check on the next iteration. At 
each process action, the health of the player is checked to see if a game 
over situation is detected.

# Death

Dying in the game is expected and recorded in a morgue file.  This file
is appended at each death indicating the player stats and network at the
time of death.

# Communication

Processes can at times communicate with the user.  Simple processes do not
communicate (other than pid related information in the console), but special 
processes can.  Special processes can communicate with the user through the 
chat window as they perform their abilities (which will provide cues to the 
player about what they do), but they can also communicate other information 
(such as taunts)

In this example, the final boss communicates its twist at the end:

```
[executive]: I will respawn once more.
```

Here, the final boss taunts the player:

```
[executive]: Your process is half health; how unfortunate.
```

And in this case, the 'fork' process indicates that it's spawning more of
its minions:

```
[fork]: More minions incoming.
```

Processes can also communicate information through the console (not chat) as
follows to provide hints on what they do:

```
fork [6421]: spawned a new minion [9531].

encrypt [3751]: encrypted process ids.

sentry [8532]: buffed defense for processes.
```

# Network Generation

A number of sample networks will exist that will be randomly populated
with processes at level start time.  This should provide some variety for
exploration.  The processes running in each node will be randomly defined
based upon the level.

This solution exists to avoid random networks that may lack connectivity
or a path from entry to exit nodes.

```
      1)                      2)                      3)

      +---+                          +---+                          +---+
      |Ent|                          |Ent|                          |Ent|
      +---+                          +---+                          +---+
        |                              |                              |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |--|   |--|   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
        |      |      |         |             |         |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |--|   |--|   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
        |      |      |                |                              |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |--|   |--|   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
        |      |      |         |             |         |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |--|   |--|   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
                      |                |                              |
                    +---+            +---+                          +---+
                    |Ext|            |Ext|                          |Ext|
                    +---+            +---+                          +---+


      4)                      5)                      6)

      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|Ent|--|   |     |Ent|--|   |--|   |     |   |--|   |  |Ent|
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
               |                              |         |      |      |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |  |   |  |   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
               |                              |         |      |      |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|   |--|   |     |   |--|   |--|   |     |   |  |   |  |   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
               |                              |         |      |      |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
      |   |--|Ext|--|   |     |Ext|--|   |--|   |     |Ext|  |   |--|   |
      +---+  +---+  +---+     +---+  +---+  +---+     +---+  +---+  +---+
```


# Proposed UI Design

```
              1         2         3         4         5         6         7        8
     123456789012345678901234567890123456789012345678901234567890123456789012345678901234
    1+-Processes------------------------------+-Network---------------------------------+
    2| PID   Name      AT  DE  LV  ATTR  ENRG |                                         |
    3| 8211  sentry     0   3   1  B D      8 |[xx.xx.xx.xx]- xx.xx.xx.xx - xx.xx.xx.xx |
    4| 1981  daemon     2   1   1   A      10 |      |             |             |      |
    5| 9921  login      3   3   1      @   20 | xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx |
    6|                                        |      |             |             |      |
    7|                                        | xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx |
    8|                                        |      |             |             |      |
    9|                                        | xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx |
   10|                                        |                                         |
   11+-Console--------------------------------+-Messages--------------------------------+
   12|                                        |                                         |
   13|                                        |                                         |
   14|                                        |                                         |
   15|                                        |                                         |
   16|                                        |                                         |
   17|                                        |                                         |
   18|                                        |                                         |
   19|                                        |                                         |
   20|                                        |                                         |
   21|                                        |                                         |
   22|                                        |                                         |
   23|                                        |                                         |
   24|$ bash 8211                             |damon attacks for 3 energy.              |
   25+----------------------------------------+-----------------------------------------+
```

# Notes

   ** For more advanced exploits, need to identify pros and cons for each (has a 
      benefit and an issue).

   ** Need to identify the balance between energy-based hacking and combat.

# Task List

- [x]  Implement the UI (ncurses-based). (Reused)

- [x]  Implement the player input. (Reused)

- [x]  Implement the process definitions, including callbacks.

- [ ]  Implement the network generation and display.

- [x]  Implement the chat window.

- [x]  Add process window and display.

- [x]  Implement the process invocation (based upon action rate).

- [ ]  Implement the basic game loop.

- [ ]  Implement the bash command.

- [ ]  Implement the minion process.

- [ ]  Implement the hack command.

- [ ]  Implement the conflict system.

- [ ]  Implement the experience and level system.

- [ ]  Implement the connect command to traverse the network.

- [ ]  Implement the wait command.

- [ ]  Implement the bash command.

- [ ]  Implement the hack command and the QTE system.

- [ ]  Implement the cut command.

- [ ]  Implement the suspend command.

- [ ]  Implement the wall command.

- [ ]  Implement the kill command.

- [ ]  Implement the trap command.

- [ ]  Implement the barrier command.

- [ ]  Implement the counter command.

- [ ]  Implement the slice command.

- [ ]  Implement the TBD buff command.

- [ ]  Implement the ulogin process callback function.

- [ ]  Implement the sentry process.

- [ ]  Implement the daemon process.

- [ ]  Implement the encrypt process.

- [ ]  Implement the watchdog process.

- [ ]  Implement the upgrade process.

- [ ]  Implement the fork process.

- [ ]  Implement the random process.

- [ ]  Implement the Executive process.

- [ ]  Implement the death process and the morgue.

- [ ]  Implement a way for processes to communicate with the user (via chat).

- [ ]  Implement network initialization via network templates.

- [ ]  Implement the end game (perhaps in the context of the Executive process).

