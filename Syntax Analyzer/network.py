# import json
# import networkx as nx
# import matplotlib.pyplot as plt

# def build_graph_from_json(json_file):
#     with open(json_file, 'r') as file:
#         data = json.load(file)

#     G = nx.DiGraph()  # Create a directed graph

#     def add_nodes_and_edges(node_data):
#         node_id = node_data["current"]
#         G.add_node(node_id, data=node_data["data"])
#         if "parent" in node_data:
#             G.add_edge(node_data["parent"], node_id)
#         for child in node_data.get("children", []):
#             add_nodes_and_edges(child)

#     add_nodes_and_edges(data)

#     return G

# def save_graph_image(json_file, output_image_path):
#     graph = build_graph_from_json(json_file)
#     plt.figure(figsize=(50, 50))  # Set the figure size
#     pos = nx.circular_layout(graph)  # Compute layout

#     # Explicitly create a figure and axes
#     fig, ax = plt.subplots(figsize=(50, 50))
#     # Draw the graph using the explicit figure and axes
#     nx.draw(graph, pos, ax=ax, with_labels=True, node_size=150, node_color="lightblue", font_size=10, font_weight="bold")

#     plt.savefig(output_image_path)  # Save the figure as an image
#     plt.close(fig)  # Close the figure to free memory

# save_graph_image('tree_structure.json', 'tree_visualization.png')
import json
from pyvis.network import Network

def build_interactive_graph_from_json(json_file, output_html_path):
    with open(json_file, 'r') as file:
        data = json.load(file)

    net = Network(notebook=True, height="750px", width="100%")  # Create an interactive network

    def add_nodes_and_edges(node_data, parent_id=None):
        node_id = node_data["current"]
        node_label = node_data["data"]
        net.add_node(node_id, label=node_label)

        if parent_id is not None:
            net.add_edge(parent_id, node_id)

        for child in node_data.get("children", []):
            add_nodes_and_edges(child, parent_id=node_id)

    add_nodes_and_edges(data)  # Assuming the JSON structure starts with a root node

    # Save or show the interactive graph
    net.show(output_html_path)

# Specify your JSON file path and the output HTML file path
build_interactive_graph_from_json('tree_structure.json', 'tree_visualization.html')
