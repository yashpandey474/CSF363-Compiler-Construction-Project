# First, we need to read the JSON data from the file 'tree_structure.json' and perform an inorder traversal to print the information as requested.
# The provided information suggests an inorder traversal format that focuses on specific attributes of each node.

import json

# Define a function to perform an inorder traversal and print the node information in the requested format.


def inorder_traversal(node, parent_symbol=""):
    # Base case: if the node is None, return
    if node is None:
        return

    # Traverse the first child (if exists)
    children = node.get("children", [])
    if children:
        inorder_traversal(children[0], parent_symbol=node.get("data"))

    # Print the current node information
    lexeme = node.get("lexeme", "-----")
    line_no = str(node.get("lineNo", -1))
    token_name = node.get("data", "-----") if node.get(
        "isleaf") == "yes" else "-----"
    value = node.get("value", "-----")
    parent_node_symbol = parent_symbol if parent_symbol else "ROOT"
    is_leaf = "yes" if node.get("isleaf") == "yes" else "no"
    node_symbol = "LEAF" if is_leaf == "yes" else node.get("data")

    print(f"(Lexeme Current Node){lexeme:<20} (Line No){line_no:<5} (Token Name){token_name:<20} (Value if number){value:<20} (Parent Node Symbol){parent_node_symbol:<20} (IS leaf node){is_leaf:<3} (Node symbol){node_symbol:<20}")

    # Traverse the remaining children
    for child in children[1:]:
        inorder_traversal(child, parent_symbol=node.get("data"))


# Load the JSON data from the file
with open('tree_structure.json', 'r') as file:
    data = json.load(file)

# Perform the inorder traversal starting from the root node
inorder_traversal(data)
