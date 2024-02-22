//2D PARSING TABLE
//NT - T TO RULE
#include "syntactical.h"
void printRule(enum NonTerminals nt, struct Variable* ruleArray)
{

  printf("%s ===> ", NonTerminalToString(nt));

  for(int i=0;i<9;i++)
  {
    if(ruleArray[i].val==0 &&ruleArray[i].flag==0)
    {
      break;
    }
          if (ruleArray[i].flag == 0){
              printf("%s ", TokenToString(ruleArray[i].val));
          }
          else{
            printf("%s ", NonTerminalToString(ruleArray[i].val));
        }
  }

  printf("\n");
}
void printParsingTable(struct ParsingTable* pt) {
  if (pt == NULL) {
    printf("ParsingTable pointer is NULL\n");
    return;
  }
  for (int i = 0; i < NUM_NON_TERMINALS; i++) {
    if (i != NT_FUNCTION){
      continue;
    }
    for (int j = 0; j < NUM_TERMINALS; j++) {

      printf("Cell[%s][%s]:", NonTerminalToString(i), TokenToString(j));
      if (pt->table[i][j] != NULL) {
        printRule(i, pt->table[i][j]);
      }
      else
      {
          printf("ERROR\n");
      }
    }
  }
}
void insert(enum NonTerminals nt, enum Tokentype terminal, struct Variable* rule, struct ParsingTable * PT)
{
    PT->table[(int)(nt)][(int)terminal] = rule;
}

void populate_parsing_table(struct ParsingTable * PT, struct GrammarRule* productions,struct Sets **sets_for_all){
    for(int nt=0;nt<NUM_NON_TERMINALS;nt++){
        for(int i=0;i<productions[nt].numProductions;i++)
        {
          //GET CURRENT RULE
            struct Variable* rule=productions[nt].rules[i];

          //EPSILON RULE
            if(rule[0].val==TK_EPS)
            {
                struct Node* current = sets_for_all[nt]->followSets->linkedList->head;

                while (current != NULL){
                    insert(nt, current->data.val, rule, PT);
                    current=current->next;
                }
            }


            else{

            int var = 0;
            for (; var < 9 && !(isDefault(rule[var])); var += 1)
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

            if (var == 9 || isDefault(rule[var])){
              //ADD THE RULE IN FOLLOW OF THE VAR [EPSILON RULE]
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