// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Agrawal Vansh Anil	     2021A7PS2215P
// Yash Pandey	             2021A7PS0661P
// Hardik Gupta 	         2021A7PS2421P
// Ujjwal Aggarwal	         2021A7PS2427P
// Achinthya Hebbar S	     2021A7PS1457P

// 2D PARSING TABLE
// NT - T TO RULE
#include "../parser.h"
void printRule(enum NonTerminals nt, struct Variable *ruleArray)
{

  printf("%s ===> ", NonTerminalToString(nt));

  for (int i = 0; i < 9; i++)
  {
    if (ruleArray[i].val == 0 && ruleArray[i].flag == 0)
    {
      break;
    }
    if (ruleArray[i].flag == 0)
    {
      printf("%s ", TokenToString(ruleArray[i].val));
    }
    else
    {
      printf("%s ", NonTerminalToString(ruleArray[i].val));
    }
  }

  printf("\n");
}
void printParsingTable(struct ParsingTable *pt)
{
  if (pt == NULL)
  {
    printf("ParsingTable pointer is NULL\n");
    return;
  }
  for (int i = 0; i < NUM_NON_TERMINALS; i++)
  {
    for (int j = 0; j < NUM_TERMINALS; j++)
    {

      printf("Cell[%s][%s]:", NonTerminalToString(i), TokenToString(j));
      if (pt->table[i][j] != NULL)
      {
        printRule(i, pt->table[i][j]);
      }
      else
      {
        printf("ERROR\n");
      }
    }
  }
}
void insert(enum NonTerminals nt, enum Tokentype terminal, struct Variable *rule, struct ParsingTable *PT)
{
  PT->table[(int)(nt)][(int)terminal] = rule;
}

void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all, int nonTerminal, int *synchSet)
{
  // SYN INDICATING RULE
  struct Variable *rule = (struct Variable *)malloc(sizeof(struct Variable));
  rule[0].val = -1;

  struct Node *current = sets_for_all[nonTerminal]->followSets->linkedList->head;

  // FOR ALL TERMINALS IN FOLLOW OF NONTERMINAL
  while (current != NULL)
  {
    // INSERT INTO PARSING TABLE
    insert(nonTerminal, current->data.val, rule, PT);
    current = current->next;
  }

  // FOR ALL IN DEFAULT SYNC SET
  for (int i = 0; i < sizeof(synchSet) / sizeof(synchSet[0]); i += 1)
  {
    insert(nonTerminal, synchSet[i], rule, PT);
  }
}

void createParseTable(struct ParsingTable *PT, struct GrammarRule *productions, struct Sets **sets_for_all, int *synchSet, int defaultSynchSetSize)
{
  for (int nt = 0; nt < NUM_NON_TERMINALS; nt++)
  {
    // ADD THE SYN INDICATOR FOR ALL TERMINALS IN FOLLOW OF NT
    addSyn(PT, sets_for_all, nt, synchSet);

    for (int i = 0; i < productions[nt].numProductions; i++)
    {
      // GET CURRENT RULE
      struct Variable *rule = productions[nt].rules[i];

      // EPSILON RULE
      if (rule[0].val == TK_EPS)
      {
        struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

        while (current != NULL)
        {
          insert(nt, current->data.val, rule, PT);
          current = current->next;
        }
      }

      else
      {

        int var = 0;
        for (; var < MAX_VARS && !(isDefault(rule[var])); var += 1)
        {

          int set_index = rule[var].val;
          if (rule[var].flag == 0)
          {
            set_index += NUM_NON_TERMINALS;
          }
          struct Node *current = sets_for_all[set_index]->firstSets->linkedList->head;

          while (current != NULL)
          {
            // DONT ADD TK_EPS
            if (current->data.val == TK_EPS)
            {
              current = current->next;
              continue;
            }
            insert(nt, current->data.val, rule, PT);
            current = current->next;
          }

          if (!(containsEPS(sets_for_all[set_index]->firstSets)))
          {
            break;
          }
        }

        if (var == 9 || isDefault(rule[var]))
        {
          // ADD THE RULE IN FOLLOW OF THE VAR [EPSILON RULE]
          struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

          while (current != NULL)
          {
            insert(nt, current->data.val, rule, PT);
            current = current->next;
          }
        }
      }
    }
  }

  return;
}