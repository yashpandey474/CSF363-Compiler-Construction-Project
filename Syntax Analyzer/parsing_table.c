//2D PARSING TABLE
//NT - T TO RULE
#include "syntactical.h"

void printParsingTable(struct ParsingTable* pt) {
  if (pt == NULL) {
    printf("ParsingTable pointer is NULL\n");
    return;
  }
  for (int i = 0; i < NUM_NON_TERMINALS; i++) {
    for (int j = 0; j < NUM_TERMINALS; j++) {
      if (pt->table[i][j] != NULL) {
        printf("Cell[%d][%d]: Val = %d, Flag = %s\n", 
               i, j, pt->table[i][j]->val, 
               (pt->table[i][j]->flag == 0) ? "TERMINAL" : "NONTERMINAL");
      } else {
        printf("Cell[%d][%d]: ERROR\n", i, j);
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
        struct GrammarRule a=productions[nt];
        for(int i=0;i<a.numProductions;i++)
        {
            struct Variable* rule=a.rules[i];
            if(rule[0].val==TK_EPS)
            {
                struct Node* current = sets_for_all[nt]->followSets->linkedList->head;

                while (current != NULL){
                    insert(nt, current->data.val, rule, PT);
                    current=current->next;
                }
            }
            else{
                int set_index = rule[0].val;
                if (rule[0].flag == 0){
                    set_index += NUM_NON_TERMINALS;
                }
                struct Node* current = sets_for_all[set_index]->firstSets->linkedList->head;

                while (current != NULL){
                    
                    insert(nt, current->data.val, rule, PT);
                    current=current->next;
                }
            }
        }
    }
    return;
}
