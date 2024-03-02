import json
from pyvis.network import Network

def build_interactive_tree_from_json(json_file, output_html_path):
    with open(json_file, 'r') as file:
        data = json.load(file)

    net = Network(notebook=True, height="750px", width="100%", directed=True)
    net.force_atlas_2based(gravity=-50, central_gravity=0.01, spring_length=100, spring_strength=0.02, damping=0.4, overlap=0)

    def add_nodes_and_edges(node_data, parent_id=None):
        node_id = node_data["current"]
        node_label = node_data["data"]
        net.add_node(node_id, label=node_label, title=node_label)

        if parent_id is not None:
            net.add_edge(parent_id, node_id)

        for child in node_data.get("children", []):
            add_nodes_and_edges(child, parent_id=node_id)

    add_nodes_and_edges(data)  # Assuming the JSON structure starts with a root node

    # Additional options for hierarchical layout
    net.set_options("""
    {
    "layout": {
        "hierarchical": {
        "enabled": true,
        "levelSeparation": 150,
        "nodeSpacing": 100,
        "treeSpacing": 200,
        "blockShifting": true,
        "edgeMinimization": true,
        "parentCentralization": true,
        "direction": "UD",
        "sortMethod": "directed"
        }
    },
    "physics": {
        "hierarchicalRepulsion": {
        "centralGravity": 0.0,
        "springLength": 100,
        "springConstant": 0.01,
        "nodeDistance": 120,
        "damping": 0.09
        }
    }
    }
    """)

    # Save or show the interactive graph
    net.show(output_html_path)

# Specify your JSON file path and the output HTML file path
build_interactive_tree_from_json('tree_structure.json', 'tree_visualization.html')
