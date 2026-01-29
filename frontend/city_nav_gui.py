"""
city_nav_gui.py
City Navigation System - Python GUI Frontend

Description: Visualization interface for city navigation
Uses: Tkinter, NetworkX, Matplotlib
Author: Dhrubajyoti Das
Date: January 2026
"""

import tkinter as tk
from tkinter import ttk, messagebox, simpledialog
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import os
from datetime import datetime
import time
import random
import math


class MultiInputDialog:
    """Custom dialog for multiple inputs"""

    def __init__(self, parent, title, fields):
        """
        fields format: [
            {'label': 'City ID:', 'type': 'int'},
            {'label': 'City Name:', 'type': 'str'},
            {'label': 'X Coordinate:', 'type': 'int'},
            {'label': 'Y Coordinate:', 'type': 'int'}
        ]
        """
        self.result = None
        self.top = tk.Toplevel(parent)
        self.top.title(title)
        self.top.geometry("350x550")  # Increased height for city list
        self.top.configure(bg="#1e1e2e")
        self.top.transient(parent)
        self.top.grab_set()

        # Center the dialog
        self.top.update_idletasks()
        x = (self.top.winfo_screenwidth() // 2) - (350 // 2)
        y = (self.top.winfo_screenheight() // 2) - (550 // 2)  # Match new height
        self.top.geometry(f"350x550+{x}+{y}")

        # Header
        header_frame = tk.Frame(self.top, bg="#89b4fa", height=60)
        header_frame.pack(fill=tk.X)
        header_frame.pack_propagate(False)

        header_label = tk.Label(
            header_frame,
            text=title,
            font=("Segoe UI", 14, "bold"),
            bg="#89b4fa",
            fg="#1e1e2e",
        )
        header_label.pack(expand=True)

        # Content frame
        content_frame = tk.Frame(self.top, bg="#1e1e2e")
        content_frame.pack(fill=tk.BOTH, expand=True, padx=20, pady=20)

        # Create input fields
        self.entries = []
        for i, field in enumerate(fields):
            label = tk.Label(
                content_frame,
                text=field["label"],
                font=("Segoe UI", 10, "bold"),
                bg="#1e1e2e",
                fg="#cdd6f4",
            )
            label.grid(row=i, column=0, sticky="w", pady=8)

            entry = tk.Entry(
                content_frame,
                font=("Segoe UI", 11),
                bg="#313244",
                fg="#cdd6f4",
                insertbackground="#cdd6f4",
                relief=tk.FLAT,
                bd=5,
            )
            entry.grid(row=i, column=1, sticky="ew", pady=8, padx=(10, 0))
            entry.field_type = field["type"]
            self.entries.append(entry)

        content_frame.columnconfigure(1, weight=1)

        # Add city list if available (passed via parent)
        if hasattr(parent, "cities") and parent.cities:
            cities_label = tk.Label(
                content_frame,
                text="📋 Available Cities:",
                font=("Segoe UI", 9, "bold"),
                bg="#1e1e2e",
                fg="#89b4fa",
            )
            cities_label.grid(
                row=len(fields), column=0, columnspan=2, sticky="w", pady=(15, 5)
            )

            # Scrollable city list
            list_frame = tk.Frame(content_frame, bg="#1e1e2e")
            list_frame.grid(
                row=len(fields) + 1, column=0, columnspan=2, sticky="nsew", pady=(0, 10)
            )

            scrollbar = tk.Scrollbar(list_frame, bg="#313244")
            scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

            city_list_text = tk.Text(
                list_frame,
                height=6,
                width=35,
                bg="#313244",
                fg="#a6e3a1",
                font=("Consolas", 9),
                yscrollcommand=scrollbar.set,
                relief=tk.FLAT,
                bd=2,
                state=tk.DISABLED,
            )
            city_list_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            scrollbar.config(command=city_list_text.yview)

            # Populate city list
            city_list_text.config(state=tk.NORMAL)
            sorted_cities = sorted(parent.cities.items(), key=lambda x: x[0])
            for city_id, data in sorted_cities:
                city_list_text.insert(tk.END, f"{city_id}. {data['name']}\n")
            city_list_text.config(state=tk.DISABLED)

            content_frame.rowconfigure(len(fields) + 1, weight=1)


        # Button frame
        button_frame = tk.Frame(self.top, bg="#1e1e2e")
        button_frame.pack(fill=tk.X, padx=20, pady=(0, 20))

        # OK button
        ok_btn = tk.Button(
            button_frame,
            text="✓ OK",
            command=self.on_ok,
            bg="#a6e3a1",
            fg="#1e1e2e",
            font=("Segoe UI", 10, "bold"),
            relief=tk.FLAT,
            cursor="hand2",
            padx=20,
            pady=8,
        )
        ok_btn.pack(side=tk.LEFT, expand=True, fill=tk.X, padx=(0, 5))

        # Cancel button
        cancel_btn = tk.Button(
            button_frame,
            text="✗ Cancel",
            command=self.on_cancel,
            bg="#f38ba8",
            fg="#1e1e2e",
            font=("Segoe UI", 10, "bold"),
            relief=tk.FLAT,
            cursor="hand2",
            padx=20,
            pady=8,
        )
        cancel_btn.pack(side=tk.RIGHT, expand=True, fill=tk.X, padx=(5, 0))

        # Focus first entry
        if self.entries:
            self.entries[0].focus()

        # Bind Enter key to OK
        self.top.bind("<Return>", lambda e: self.on_ok())
        self.top.bind("<Escape>", lambda e: self.on_cancel())

    def on_ok(self):
        """Validate and collect results"""
        results = []
        for entry in self.entries:
            value = entry.get().strip()

            if not value:
                messagebox.showerror(
                    "Error", "All fields are required!", parent=self.top
                )
                entry.focus()
                return

            try:
                if entry.field_type == "int":
                    results.append(int(value))
                else:
                    results.append(value)
            except ValueError:
                messagebox.showerror(
                    "Error", f"Invalid number: {value}", parent=self.top
                )
                entry.focus()
                return

        self.result = results
        self.top.destroy()

    def on_cancel(self):
        """Cancel dialog"""
        self.result = None
        self.top.destroy()

    def show(self):
        """Show dialog and wait for result"""
        self.top.wait_window()
        return self.result


class CityNavigationGUI:
    """Main GUI class for City Navigation System"""

    def __init__(self, root):
        self.root = root
        self.root.title("🗺️ City Navigation System - Professional Edition")
        self.root.geometry("1920x1080")
        self.root.configure(bg="#1e1e2e")

        # Graph data structures
        self.graph = nx.DiGraph()
        self.pos = {}
        self.cities = {}
        self.highlighted_path = []

        # Dataset storage (for random generation) 
        self.dataset_cities = {}  

        # Setup UI first, then load data, then draw
        self.setup_ui()
        self.load_dataset()  
        self.load_data()  # Load active map
        self.draw_graph()

    def setup_ui(self):
        """Setup the user interface"""
        # Style configuration
        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TFrame", background="#1e1e2e")
        style.configure(
            "TLabel", background="#1e1e2e", foreground="#cdd6f4", font=("Segoe UI", 10)
        )
        style.configure("TButton", font=("Segoe UI", 9, "bold"))
        style.configure(
            "Header.TLabel", font=("Segoe UI", 16, "bold"), foreground="#89b4fa"
        )

        # Main container
        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # LEFT PANEL - Controls 
        left_panel = ttk.Frame(main_frame, width=300)
        left_panel.pack(side=tk.LEFT, fill=tk.BOTH, padx=(0, 10))
        left_panel.pack_propagate(False)

        # Header
        header = ttk.Label(left_panel, text="🗺️ City Navigation", style="Header.TLabel")
        header.pack(pady=10)

        # Control buttons
        self.create_control_buttons(left_panel)

        # Info panel
        info_label = ttk.Label(
            left_panel,
            text="📋 Operation Log",
            font=("Segoe UI", 12, "bold"),
            foreground="#89b4fa",
        )
        info_label.pack(pady=(10, 5))

        self.info_text = tk.Text(
            left_panel,
            height=10,
            width=32,
            bg="#313244",
            fg="#cdd6f4",
            font=("Consolas", 9),
            wrap=tk.WORD,
            borderwidth=2,
            relief=tk.GROOVE,
        )
        self.info_text.pack(pady=5, padx=10, fill=tk.BOTH, expand=True)

        # Status bar at bottom
        self.status_label = ttk.Label(
            left_panel,
            text="✅ Ready",
            font=("Segoe UI", 9, "bold"),
            foreground="#a6e3a1",
        )
        self.status_label.pack(side=tk.BOTTOM, pady=5)

        # CENTER PANEL - Graph Visualization 
        center_panel = ttk.Frame(main_frame)
        center_panel.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 10))

        # Graph canvas
        self.fig = Figure(figsize=(14, 8), dpi=120, facecolor="#1e1e2e")
        self.ax = self.fig.add_subplot(111)
        self.ax.set_facecolor("#181825")

        self.canvas = FigureCanvasTkAgg(self.fig, center_panel)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        # RIGHT PANEL - City Directory
        right_panel = ttk.Frame(main_frame, width=280)
        right_panel.pack(side=tk.RIGHT, fill=tk.BOTH)
        right_panel.pack_propagate(False)

        # City directory header with colorful background
        city_header_frame = tk.Frame(right_panel, bg="#89b4fa", height=50)
        city_header_frame.pack(fill=tk.X, padx=10, pady=(10, 5))
        city_header_frame.pack_propagate(False)

        city_header = tk.Label(
            city_header_frame,
            text="🏙️ City Directory",
            font=("Segoe UI", 14, "bold"),
            bg="#89b4fa",
            fg="#1e1e2e",
        )
        city_header.pack(expand=True)

        # City listbox with scrollbar
        list_frame = ttk.Frame(right_panel)
        list_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        scrollbar = ttk.Scrollbar(list_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.city_listbox = tk.Listbox(
            list_frame,
            bg="#313244",
            fg="#a6e3a1",
            font=("Consolas", 11, "bold"),
            selectmode=tk.SINGLE,
            yscrollcommand=scrollbar.set,
            borderwidth=3,
            relief=tk.GROOVE,
            highlightthickness=0,
            selectbackground="#f9e2af",
            selectforeground="#1e1e2e",
        )
        self.city_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.config(command=self.city_listbox.yview)

        # City info box with colorful header
        info_header_frame = tk.Frame(right_panel, bg="#f38ba8", height=40)
        info_header_frame.pack(fill=tk.X, padx=10, pady=(10, 0))
        info_header_frame.pack_propagate(False)

        info_header = tk.Label(
            info_header_frame,
            text="📍 Selected City Info",
            font=("Segoe UI", 11, "bold"),
            bg="#f38ba8",
            fg="#1e1e2e",
        )
        info_header.pack(expand=True)

        self.city_info_text = tk.Text(
            right_panel,
            height=8,
            bg="#313244",
            fg="#cdd6f4",
            font=("Consolas", 9),
            wrap=tk.WORD,
            borderwidth=3,
            relief=tk.GROOVE,
            state=tk.DISABLED,
        )

        self.city_info_text.pack(fill=tk.X, padx=10, pady=(0, 10))

        # Bind selection event
        self.city_listbox.bind("<<ListboxSelect>>", self.on_city_select)

    def create_control_buttons(self, parent):
        """Create control buttons"""
        buttons_frame = ttk.Frame(parent)
        buttons_frame.pack(pady=10, padx=10, fill=tk.X)

        buttons = [
            ("🔍 Find Path (Dijkstra)", self.find_shortest_path_dijkstra, "#89b4fa"),
            ("⭐ Find Path (A*)", self.find_shortest_path_astar, "#f38ba8"),
            ("🌊 BFS Traversal", self.bfs_traversal, "#a6e3a1"),
            ("🌲 DFS Traversal", self.dfs_traversal, "#fab387"),
            ("🎲 Generate Random Map", self.generate_random_map, "#cba6f7"),
            ("🔄 Load Default Map", self.load_default_map, "#94e2d5"),
            ("➕ Add City", self.add_city, "#94e2d5"),
            ("➕ Add Road", self.add_road, "#cba6f7"),
            ("🗑️ Delete City", self.delete_city, "#f38ba8"),
            ("📊 Statistics", self.show_statistics, "#f9e2af"),
            ("💾 Save to Files", self.save_to_backend, "#a6e3a1"),
        ]

        for text, command, color in buttons:
            btn = tk.Button(
                buttons_frame,
                text=text,
                command=command,
                bg=color,
                fg="#1e1e2e",
                font=("Segoe UI", 8, "bold"),
                relief=tk.FLAT,
                cursor="hand2",
                padx=8,
                pady=6,
            )
            btn.pack(fill=tk.X, pady=2)
            btn.bind("<Enter>", lambda e, b=btn: b.config(bg="#cdd6f4"))
            btn.bind("<Leave>", lambda e, b=btn, c=color: b.config(bg=c))

    def update_city_list(self):
        """Update the city listbox"""
        self.city_listbox.delete(0, tk.END)
        sorted_cities = sorted(self.cities.items(), key=lambda x: x[0])
        for city_id, data in sorted_cities:
            self.city_listbox.insert(tk.END, f"{city_id}. {data['name']}")

    def on_city_select(self, event):
        """Handle city selection from list"""
        selection = self.city_listbox.curselection()
        if not selection:
            return

        index = selection[0]
        sorted_cities = sorted(self.cities.items(), key=lambda x: x[0])
        city_id, data = sorted_cities[index]

        # Get outgoing connections
        outgoing = list(self.graph.successors(city_id))

        # Update info display
        self.city_info_text.config(state=tk.NORMAL)
        self.city_info_text.delete(1.0, tk.END)

        info = f"City ID: {city_id}\n"
        info += f"Name: {data['name']}\n"
        info += f"Position: ({data['x']}, {data['y']})\n"
        info += f"\nOutgoing Roads: {len(outgoing)}\n"

        if outgoing:
            info += "\n━━━ Connections ━━━\n"
            for dest_id in outgoing:
                dest_name = self.cities[dest_id]["name"]
                distance = self.graph[city_id][dest_id]["weight"]
                info += f"→ {dest_name} ({distance} km)\n"
        else:
            info += "\nNo outgoing roads"

        self.city_info_text.insert(1.0, info)
        self.city_info_text.config(state=tk.DISABLED)

    def load_data(self):
        """Load cities and roads from text files"""
        try:
            if not os.path.exists("data/cities.txt"):
                self.log_info("⚠️ data/cities.txt not found!")
                return

            if not os.path.exists("data/roads.txt"):
                self.log_info("⚠️ data/roads.txt not found!")
                return

            # Clear existing data  
            self.graph.clear()
            self.cities.clear()
            self.pos.clear()

            # Load cities
            with open("data/cities.txt", "r") as f:
                next(f)
                for line in f:
                    parts = line.strip().split(",")
                    if len(parts) == 4:
                        city_id = int(parts[0])
                        name = parts[1]
                        x = int(parts[2])
                        y = int(parts[3])
                        self.cities[city_id] = {"name": name, "x": x, "y": y}
                        self.graph.add_node(city_id, name=name, pos=(x, y))
                        self.pos[city_id] = (x, y)

            # Load roads
            with open("data/roads.txt", "r") as f:
                next(f)
                for line in f:
                    parts = line.strip().split(",")
                    if len(parts) == 3:
                        from_id = int(parts[0])
                        to_id = int(parts[1])
                        dist = int(parts[2])
                        self.graph.add_edge(from_id, to_id, weight=dist)

            self.log_info(
                f"✅ Loaded {len(self.cities)} cities and {self.graph.number_of_edges()} roads"
            )
            self.status_label.config(
                text=f"✅ Loaded {len(self.cities)} cities, {self.graph.number_of_edges()} roads"
            )
            self.update_city_list()

        except Exception as e:
            self.log_info(f"❌ Error: {e}")

    # RANDOM MAP GENERATION 
    def load_dataset(self):
        """Load the full 60-city dataset for random generation"""
        try:
            if not os.path.exists("data/dataset_cities.txt"):
                self.log_info("⚠️ Dataset not found. Random generation disabled.")
                return

            with open("data/dataset_cities.txt", "r") as f:
                next(f)  # Skip header
                for line in f:
                    parts = line.strip().split(",")
                    if len(parts) == 4:
                        city_id = int(parts[0])
                        name = parts[1]
                        self.dataset_cities[city_id] = name

            self.log_info(
                f"📚 Dataset loaded: {len(self.dataset_cities)} cities available"
            )

        except Exception as e:
            self.log_info(f"⚠️ Dataset load error: {e}")

    def generate_random_map(self):
        """Generate a random 8-city, 13-edge map"""
        if not self.dataset_cities:
            messagebox.showerror(
                "Error", "Dataset not loaded! Place dataset_cities.txt in data/ folder."
            )
            return

        if len(self.dataset_cities) < 8:
            messagebox.showerror("Error", "Dataset must have at least 8 cities!")
            return

        self.log_info("\n🎲 GENERATING RANDOM MAP\n" + "=" * 40)
        self.status_label.config(text="⏳ Generating...")
        self.root.update()

        try:
            # Step 1: Select 8 random cities
            selected_ids = random.sample(list(self.dataset_cities.keys()), 8)
            selected_cities = {
                i + 1: self.dataset_cities[city_id]
                for i, city_id in enumerate(selected_ids)
            }

            self.log_info(f"Selected: {', '.join(selected_cities.values())}")

            # Step 2: Generate coordinates with proper spacing
            coordinates = self.generate_coordinates_with_spacing(8, min_distance=150)

            if not coordinates:
                messagebox.showerror("Error", "Failed to generate valid coordinates!")
                self.status_label.config(text="❌ Generation failed")
                return

            # Step 3: Create connected graph with 13 edges
            edges_with_distances = self.create_connected_graph(
                coordinates, num_edges=13
            )

            # Step 4: Save to files
            self.save_generated_map(selected_cities, coordinates, edges_with_distances)

            # Step 5: Reload GUI
            self.load_data()
            self.draw_graph()

            self.log_info("✅ Random map generated successfully!")
            self.status_label.config(text="✅ Random map loaded")

            messagebox.showinfo(
                "Success",
                "Random 8-city map generated!\n\nCities have been randomly selected and positioned.",
            )

        except Exception as e:
            self.log_info(f"❌ Generation error: {e}")
            messagebox.showerror("Error", f"Generation failed: {e}")
            self.status_label.config(text="❌ Generation failed")

    def generate_coordinates_with_spacing(self, num_cities, min_distance=150):
        """Generate non-overlapping coordinates for cities"""
        coordinates = []
        canvas_size = 640
        margin = 80
        max_attempts = 1000

        for i in range(num_cities):
            attempts = 0
            while attempts < max_attempts:
                # Generate random coordinate
                x = random.randint(margin, canvas_size - margin)
                y = random.randint(margin, canvas_size - margin)

                # Check distance from all existing coordinates
                valid = True
                for existing_x, existing_y in coordinates:
                    distance = math.sqrt((x - existing_x) ** 2 + (y - existing_y) ** 2)
                    if distance < min_distance:
                        valid = False
                        break

                if valid:
                    coordinates.append((x, y))
                    break

                attempts += 1

            if attempts >= max_attempts:
                # Fallback: use grid-based placement
                self.log_info(f"⚠️ Using fallback grid placement for city {i+1}")
                return self.generate_grid_coordinates(num_cities)

        return coordinates

    def generate_grid_coordinates(self, num_cities):
        """Fallback: Generate coordinates on a grid"""
        coordinates = []
        grid_size = 3  # 3x3 grid
        spacing = 200
        start_x = 120
        start_y = 120

        positions = [
            (start_x + i * spacing, start_y + j * spacing)
            for i in range(grid_size)
            for j in range(grid_size)
        ]

        # Shuffle and take first num_cities
        random.shuffle(positions)
        return positions[:num_cities]

    def create_connected_graph(self, coordinates, num_edges=13):
        """Create a connected graph with exactly num_edges edges"""
        num_cities = len(coordinates)

        # Create temporary graph with city IDs 1-8
        temp_graph = nx.Graph()
        for i in range(1, num_cities + 1):
            temp_graph.add_node(i)

        # Step 1: Create Minimum Spanning Tree (ensures connectivity)
        # We need 7 edges to connect 8 cities
        edges = []
        connected = {1}  # Start with city 1
        unconnected = set(range(2, num_cities + 1))

        # Prim's algorithm for MST
        while unconnected:
            best_edge = None
            best_distance = float("inf")

            for c in connected:
                for u in unconnected:
                    dist = self.calculate_distance(
                        coordinates[c - 1], coordinates[u - 1]
                    )
                    if dist < best_distance:
                        best_distance = dist
                        best_edge = (c, u, dist)

            if best_edge:
                c, u, dist = best_edge
                edges.append(best_edge)
                connected.add(u)
                unconnected.remove(u)

        # Step 2: Add remaining edges randomly (13 - 7 = 6 more edges)
        remaining_edges_needed = num_edges - len(edges)

        # Get all possible edges that don't already exist
        existing_pairs = {(min(u, v), max(u, v)) for u, v, _ in edges}

        all_possible = []
        for i in range(1, num_cities + 1):
            for j in range(i + 1, num_cities + 1):
                pair = (i, j)
                if pair not in existing_pairs:
                    dist = self.calculate_distance(
                        coordinates[i - 1], coordinates[j - 1]
                    )
                    all_possible.append((i, j, dist))

        # Randomly select additional edges
        if len(all_possible) >= remaining_edges_needed:
            additional = random.sample(all_possible, remaining_edges_needed)
            edges.extend(additional)
        else:
            edges.extend(all_possible) 

        return edges

    def calculate_distance(self, coord1, coord2):
        """Calculate Euclidean distance between two coordinates"""
        x1, y1 = coord1
        x2, y2 = coord2
        return int(math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2))

    def save_generated_map(self, cities, coordinates, edges):
        """Save generated map to cities.txt and roads.txt"""
        # Save cities
        with open("data/cities.txt", "w") as f:
            f.write("CityID,CityName,X_Coord,Y_Coord\n")
            for city_id in sorted(cities.keys()):
                name = cities[city_id]
                x, y = coordinates[city_id - 1]
                f.write(f"{city_id},{name},{x},{y}\n")

        # Save roads
        with open("data/roads.txt", "w") as f:
            f.write("FromCityID,ToCityID,Distance\n")
            for from_id, to_id, distance in edges:
                f.write(f"{from_id},{to_id},{distance}\n")

        self.log_info(f"💾 Saved {len(cities)} cities and {len(edges)} roads")

    def load_default_map(self):
        """Restore the default 8-city map"""
        if not os.path.exists("data/default_cities.txt") or not os.path.exists(
            "data/default_roads.txt"
        ):
            messagebox.showerror(
                "Error",
                "Default map files not found!\n\nExpected:\n- data/default_cities.txt\n- data/default_roads.txt",
            )
            return

        try:
            # Copy default files to active files
            import shutil

            shutil.copy("data/default_cities.txt", "data/cities.txt")
            shutil.copy("data/default_roads.txt", "data/roads.txt")

            self.load_data()
            self.draw_graph()

            self.log_info("🔄 Default map restored!")
            self.status_label.config(text="✅ Default map loaded")
            messagebox.showinfo("Success", "Default map has been restored!")

        except Exception as e:
            messagebox.showerror("Error", f"Failed to load default map: {e}")

    # END: RANDOM MAP GENERATION

    def parse_city_input(self, user_input):
        """Parse user input - accepts either city ID (int) or city name (string)
        Returns city ID if found, None otherwise

        Examples:
            "5" or 5 → returns 5
            "Mumbai" or "mumbai" → returns 5 (if Mumbai has ID 5)
            "mum" → returns 5 (partial match)
        """
        if not user_input:
            return None

        user_input_str = str(user_input).strip()

        # Try parsing as integer (city ID)
        try:
            city_id = int(user_input_str)
            if city_id in self.cities:
                return city_id
            else:
                return None
        except ValueError:
            pass

        # Try matching as city name (case-insensitive, partial match)
        user_input_lower = user_input_str.lower()

        # First try exact match (case-insensitive)
        for city_id, data in self.cities.items():
            if data["name"].lower() == user_input_lower:
                return city_id

        # Then try partial match
        matches = []
        for city_id, data in self.cities.items():
            if user_input_lower in data["name"].lower():
                matches.append((city_id, data["name"]))

        # If exactly one match, return it
        if len(matches) == 1:
            return matches[0][0]

        # If multiple matches, return None (ambiguous)
        # Could show a selection dialog here if needed
        return None

    def draw_graph(self, highlight_path=None, highlight_edges=None):
        """Draw the network graph with enhanced visuals

        Args:
            highlight_path: List of nodes forming a sequential path (for Dijkstra/A*)
            highlight_edges: List of (u,v) edge tuples to highlight (for BFS/DFS tree)
        """
        self.ax.clear()

        if not self.graph.nodes():
            self.ax.text(
                0.5,
                0.5,
                "No data loaded\n\nPlease check data/cities.txt and data/roads.txt",
                ha="center",
                va="center",
                fontsize=16,
                color="#cdd6f4",
            )
            self.canvas.draw()
            return

        # Determine which nodes to highlight
        highlighted_nodes = set()
        if highlight_path:
            highlighted_nodes = set(highlight_path)
        elif highlight_edges:
            for u, v in highlight_edges:
                highlighted_nodes.add(u)
                highlighted_nodes.add(v)

        # Node colors and sizes
        node_colors = []
        node_sizes = []
        for node in self.graph.nodes():
            if node in highlighted_nodes:
                if highlight_path and (
                    node == highlight_path[0] or node == highlight_path[-1]
                ):
                    node_colors.append("#f9e2af")  # Yellow for start/end
                elif (
                    highlight_edges and node == list(highlight_edges)[0][0]
                    if highlight_edges
                    else False
                ):
                    node_colors.append("#f9e2af")  # Yellow for BFS/DFS start
                else:
                    node_colors.append("#f38ba8")  # Red for visited nodes
                node_sizes.append(3400)
            else:
                node_colors.append("#89b4fa")  # Blue for normal/unvisited
                node_sizes.append(3000)

        # Separate highlighted edges from normal edges
        path_edges = []
        non_path_edges = []

        # Convert highlight_edges to set for O(1) lookup
        highlight_edges_set = set(highlight_edges) if highlight_edges else set()

        for u, v in self.graph.edges():
            if highlight_edges and (u, v) in highlight_edges_set:
                # Direct match for BFS/DFS tree edges
                path_edges.append((u, v))
            elif highlight_path and u in highlighted_nodes and v in highlighted_nodes:
                # Sequential path check for Dijkstra/A*
                try:
                    idx_u = highlight_path.index(u)
                    idx_v = highlight_path.index(v)
                    if idx_v == idx_u + 1: 
                        path_edges.append((u, v))
                    else:
                        non_path_edges.append((u, v))
                except ValueError:
                    non_path_edges.append((u, v))
            else:
                non_path_edges.append((u, v))

        # Draw non-path edges
        if non_path_edges:
            edge_color = "#b0b2bf" if highlight_path else "#ffffff"
            edge_width = 1.5 if highlight_path else 2.5
            edge_alpha = 0.6 if highlight_path else 0.8
            nx.draw_networkx_edges(
                self.graph,
                self.pos,
                edgelist=non_path_edges,
                edge_color=edge_color,
                width=edge_width,
                ax=self.ax,
                alpha=edge_alpha,
                arrows=True,
                arrowsize=25,
                arrowstyle="->",
                connectionstyle="arc3,rad=0.20",
                min_source_margin=24,
                min_target_margin=24,
            )

        # Draw path edges - BRIGHT YELLOW
        if path_edges:
            nx.draw_networkx_edges(
                self.graph,
                self.pos,
                edgelist=path_edges,
                edge_color="#f9e2af",
                width=5,
                ax=self.ax,
                alpha=0.95,
                arrows=True,
                arrowsize=32,
                arrowstyle="->",
                connectionstyle="arc3,rad=0.20",
                min_source_margin=25,
                min_target_margin=25,
            )

        # Draw nodes with WHITE OUTLINE
        nx.draw_networkx_nodes(
            self.graph,
            self.pos,
            node_color=node_colors,
            node_size=node_sizes,
            ax=self.ax,
            alpha=0.95,
            edgecolors="#ffffff",
            linewidths=3,
        )

        # Draw city name labels
        labels = {node: self.cities[node]["name"] for node in self.graph.nodes()}
        nx.draw_networkx_labels(
            self.graph,
            self.pos,
            labels,
            font_size=8,
            font_color="#0F0E0E",
            font_weight="bold",
            ax=self.ax,
        )

        # Draw edge labels (distances)
        edge_labels = nx.get_edge_attributes(self.graph, "weight")
        rad = 0.20  
        for edge, label in edge_labels.items():
            u, v = edge
            xu, yu = self.pos[u]
            xv, yv = self.pos[v]

            # Straight-line midpoint
            mx = (xu + xv) / 2
            my = (yu + yv) / 2

            # Edge length
            edge_len = ((xv - xu) ** 2 + (yv - yu) ** 2) ** 0.5
            if edge_len == 0:
                continue

            # Perpendicular direction
            perp_x = -(yv - yu) / edge_len
            perp_y = (xv - xu) / edge_len

            # Arc3 Bezier curve math:
            # - Control point is at: midpoint + (rad * edge_len) perpendicular offset
            # - Curve midpoint at t=0.5: midpoint + 0.5 * (rad * edge_len) perpendicular offset
            curve_offset = 0.5 * rad * edge_len

            # Place label on the curve
            label_x = mx + perp_x * curve_offset
            label_y = my + perp_y * curve_offset

            # normal and path style
            if path_edges and edge in path_edges:
                color = "#a6e3a1"
                size = 13
                alpha = 0.95
            else:
                color = "#94e2d5"
                size = 12
                alpha = 0.9

            self.ax.text(
                label_x,
                label_y,
                str(int(label)),
                fontsize=size,
                color=color,
                ha="center",
                va="center",
                fontweight="bold",
                bbox=dict(
                    boxstyle="round,pad=0.3",
                    facecolor="#1e1e2e",
                    edgecolor="white",
                    alpha=alpha,
                    linewidth=0.5,
                ),
            )

        self.ax.set_title(
            "City Navigation Network",
            color="#cdd6f4",
            fontsize=16,
            pad=20,
            weight="bold",
        )
        self.ax.axis("off")
        self.ax.margins(0.15)
        self.canvas.draw()

    def find_shortest_path_dijkstra(self):
        """Find shortest path using Dijkstra's algorithm"""
        dialog = MultiInputDialog(
            self.root,
            "🔍 Find Path - Dijkstra",
            [
                {"label": "Source (ID or Name):", "type": "str"},
                {"label": "Destination (ID or Name):", "type": "str"},
            ],
        )
        result = dialog.show()

        if not result:
            return

        source_input, dest_input = result

        # Parse inputs (accept both ID and name)
        source = self.parse_city_input(source_input)
        dest = self.parse_city_input(dest_input)

        if source is None:
            messagebox.showerror(
                "Error",
                f"Source city '{source_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        if dest is None:
            messagebox.showerror(
                "Error",
                f"Destination city '{dest_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        try:
            start_time = time.time()
            path = nx.shortest_path(self.graph, source, dest, weight="weight")
            length = nx.shortest_path_length(self.graph, source, dest, weight="weight")
            end_time = time.time()

            exec_time = (end_time - start_time) * 1000

            path_names = [self.cities[c]["name"] for c in path]
            self.log_info(f"\n🔍 DIJKSTRA'S ALGORITHM\n{'='*40}")
            self.log_info(f"Source: {self.cities[source]['name']}")
            self.log_info(f"Destination: {self.cities[dest]['name']}")
            self.log_info(f"Distance: {length} km")
            self.log_info(f"Path: {' → '.join(path_names)}")
            self.log_info(f"Time: {exec_time:.3f} ms")
            self.log_info(f"Complexity: O((V+E) log V)")

            self.draw_graph(highlight_path=path)
            self.status_label.config(
                text=f"Dijkstra | {exec_time:.2f} ms | {length} km"
            )
            self.save_log("Dijkstra", source, dest, length)

        except nx.NetworkXNoPath:
            messagebox.showerror("Error", "No path exists between these cities!")
            self.status_label.config(text="Error: No path found")
        except Exception as e:
            messagebox.showerror("Error", str(e))
            self.status_label.config(text="Error occurred")

    def find_shortest_path_astar(self):
        """Find shortest path using A* algorithm"""
        dialog = MultiInputDialog(
            self.root,
            "⭐ Find Path - A*",
            [
                {"label": "Source (ID or Name):", "type": "str"},
                {"label": "Destination (ID or Name):", "type": "str"},
            ],
        )
        result = dialog.show()

        if not result:
            return

        source_input, dest_input = result

        # Parse inputs (accept both ID and name)
        source = self.parse_city_input(source_input)
        dest = self.parse_city_input(dest_input)

        if source is None:
            messagebox.showerror(
                "Error",
                f"Source city '{source_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        if dest is None:
            messagebox.showerror(
                "Error",
                f"Destination city '{dest_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        try:

            def heuristic(u, v):
                x1, y1 = self.pos[u]
                x2, y2 = self.pos[v]
                return ((x2 - x1) ** 2 + (y2 - y1) ** 2) ** 0.5

            start_time = time.time()
            path = nx.astar_path(
                self.graph, source, dest, heuristic=heuristic, weight="weight"
            )
            length = nx.astar_path_length(
                self.graph, source, dest, heuristic=heuristic, weight="weight"
            )
            end_time = time.time()

            exec_time = (end_time - start_time) * 1000

            path_names = [self.cities[c]["name"] for c in path]
            self.log_info(f"\n⭐ A* ALGORITHM\n{'='*40}")
            self.log_info(f"Source: {self.cities[source]['name']}")
            self.log_info(f"Destination: {self.cities[dest]['name']}")
            self.log_info(f"Distance: {int(length)} km")
            self.log_info(f"Path: {' → '.join(path_names)}")
            self.log_info(f"Time: {exec_time:.3f} ms")
            self.log_info(f"Complexity: O(E)")

            self.draw_graph(highlight_path=path)
            self.status_label.config(text=f"A* | {exec_time:.2f} ms | {int(length)} km")
            self.save_log("A*", source, dest, int(length))

        except nx.NetworkXNoPath:
            messagebox.showerror("Error", "No path exists between these cities!")
            self.status_label.config(text="Error: No path found")
        except Exception as e:
            messagebox.showerror("Error", str(e))
            self.status_label.config(text="Error occurred")

    def bfs_traversal(self):
        """BFS traversal - shows actual tree edges used during traversal"""
        dialog = MultiInputDialog(
            self.root,
            "🌊 BFS Traversal",
            [{"label": "Start City (ID or Name):", "type": "str"}],
        )
        result = dialog.show()

        if not result:
            return

        start_input = result[0]

        # Parse input (accept both ID and name)
        start = self.parse_city_input(start_input)

        if start is None:
            messagebox.showerror(
                "Error",
                f"City '{start_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        try:
            start_time = time.time()

            # Get BFS tree edges (the actual edges traversed)
            bfs_tree_edges = list(nx.bfs_edges(self.graph, start))

            # Get visited nodes in BFS order
            bfs_order = [start]
            for u, v in bfs_tree_edges:
                bfs_order.append(v)

            end_time = time.time()

            exec_time = (end_time - start_time) * 1000
            bfs_names = [self.cities[c]["name"] for c in bfs_order]

            # Check for unreachable nodes
            all_nodes = set(self.graph.nodes())
            visited_nodes = set(bfs_order)
            unreachable = all_nodes - visited_nodes

            self.log_info(f"\n🌊 BFS TRAVERSAL\n{'='*40}")
            self.log_info(f"Start: {self.cities[start]['name']}")
            self.log_info(f"Visited: {len(bfs_order)}/{len(all_nodes)} cities")
            self.log_info(f"Order: {' → '.join(bfs_names)}")
            self.log_info(f"Tree Edges: {len(bfs_tree_edges)}")

            if unreachable:
                unreachable_names = [self.cities[n]["name"] for n in unreachable]
                self.log_info(f"⚠️ Unreachable: {', '.join(unreachable_names)}")

            self.log_info(f"Time: {exec_time:.3f} ms")
            self.log_info(f"Complexity: O(V+E)")

            # Visualize with actual tree edges
            self.draw_graph(highlight_edges=bfs_tree_edges)
            self.status_label.config(
                text=f"BFS | {len(bfs_order)} visited | {exec_time:.2f} ms"
            )
        except Exception as e:
            messagebox.showerror("Error", str(e))
            self.status_label.config(text="Error occurred")

    def dfs_traversal(self):
        """DFS traversal - shows actual tree edges used during traversal"""
        dialog = MultiInputDialog(
            self.root,
            "🌲 DFS Traversal",
            [{"label": "Start City (ID or Name):", "type": "str"}],
        )
        result = dialog.show()

        if not result:
            return

        start_input = result[0]

        # Parse input (accept both ID and name)
        start = self.parse_city_input(start_input)

        if start is None:
            messagebox.showerror(
                "Error",
                f"City '{start_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        try:
            start_time = time.time()

            # Get DFS tree edges (the actual edges traversed)
            dfs_tree_edges = list(nx.dfs_edges(self.graph, start))

            # Get visited nodes in DFS order
            dfs_order = [start]
            for u, v in dfs_tree_edges:
                if v not in dfs_order:
                    dfs_order.append(v)

            end_time = time.time()

            exec_time = (end_time - start_time) * 1000
            dfs_names = [self.cities[c]["name"] for c in dfs_order]

            # Check for unreachable nodes
            all_nodes = set(self.graph.nodes())
            visited_nodes = set(dfs_order)
            unreachable = all_nodes - visited_nodes

            self.log_info(f"\n🌲 DFS TRAVERSAL\n{'='*40}")
            self.log_info(f"Start: {self.cities[start]['name']}")
            self.log_info(f"Visited: {len(dfs_order)}/{len(all_nodes)} cities")
            self.log_info(f"Order: {' → '.join(dfs_names)}")
            self.log_info(f"Tree Edges: {len(dfs_tree_edges)}")

            if unreachable:
                unreachable_names = [self.cities[n]["name"] for n in unreachable]
                self.log_info(f"⚠️ Unreachable: {', '.join(unreachable_names)}")

            self.log_info(f"Time: {exec_time:.3f} ms")
            self.log_info(f"Complexity: O(V+E)")

            # Visualize with actual tree edges
            self.draw_graph(highlight_edges=dfs_tree_edges)
            self.status_label.config(
                text=f"DFS | {len(dfs_order)} visited | {exec_time:.2f} ms"
            )
        except Exception as e:
            messagebox.showerror("Error", str(e))
            self.status_label.config(text="Error occurred")

    def add_city(self):
        """Add a new city"""
        dialog = MultiInputDialog(
            self.root,
            "➕ Add New City",
            [
                {"label": "City ID:", "type": "int"},
                {"label": "City Name:", "type": "str"},
                {"label": "X Coordinate (0-640):", "type": "int"},
                {"label": "Y Coordinate (0-640):", "type": "int"},
            ],
        )
        result = dialog.show()

        if not result:
            return

        city_id, name, x, y = result

        if city_id in self.cities:
            messagebox.showerror("Error", f"City ID {city_id} already exists!")
            return

        if not (0 <= x <= 640 and 0 <= y <= 640):
            messagebox.showerror("Error", "Coordinates must be between 0 and 640!")
            return

        self.cities[city_id] = {"name": name, "x": x, "y": y}
        self.graph.add_node(city_id, name=name, pos=(x, y))
        self.pos[city_id] = (x, y)

        self.log_info(f"✅ Added city: {name} (ID: {city_id})")
        self.update_city_list()
        self.draw_graph()
        self.status_label.config(text=f"Added: {name}")

    def add_road(self):
        """Add a new road"""
        dialog = MultiInputDialog(
            self.root,
            "➕ Add New Road",
            [
                {"label": "From City (ID or Name):", "type": "str"},
                {"label": "To City (ID or Name):", "type": "str"},
                {"label": "Distance (km):", "type": "int"},
            ],
        )
        result = dialog.show()

        if not result:
            return

        from_input, to_input, distance = result

        # Parse inputs (accept both ID and name)
        from_id = self.parse_city_input(from_input)
        to_id = self.parse_city_input(to_input)

        if from_id is None:
            messagebox.showerror(
                "Error",
                f"From city '{from_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        if to_id is None:
            messagebox.showerror(
                "Error",
                f"To city '{to_input}' not found!\n\nPlease use exact ID or city name.",
            )
            return

        if distance <= 0:
            messagebox.showerror("Error", "Distance must be positive!")
            return

        self.graph.add_edge(from_id, to_id, weight=distance)

        from_name = self.cities[from_id]["name"]
        to_name = self.cities[to_id]["name"]
        self.log_info(f"✅ Road: {from_name} → {to_name} ({distance} km)")
        self.draw_graph()
        self.status_label.config(text=f"Added road: {from_name} → {to_name}")

    def delete_city(self):
        """Delete a city"""
        dialog = MultiInputDialog(
            self.root, "🗑️ Delete City", [{"label": "City ID to Delete:", "type": "int"}]
        )
        result = dialog.show()

        if not result:
            return

        city_id = result[0]

        if city_id not in self.cities:
            messagebox.showerror("Error", f"City ID {city_id} not found!")
            return

        name = self.cities[city_id]["name"]

        # Confirm deletion
        confirm = messagebox.askyesno(
            "Confirm Delete", f"Delete city '{name}' and all its roads?"
        )
        if not confirm:
            return

        self.graph.remove_node(city_id)
        del self.cities[city_id]
        del self.pos[city_id]

        self.log_info(f"🗑️ Deleted: {name}")
        self.update_city_list()
        self.draw_graph()
        self.status_label.config(text=f"Deleted: {name}")

    def reload_data(self):
        """Reload data"""
        self.graph.clear()
        self.cities.clear()
        self.pos.clear()
        self.load_data()
        self.draw_graph()
        self.status_label.config(text="Data reloaded")

    def show_statistics(self):
        """Show statistics"""
        num_cities = self.graph.number_of_nodes()
        num_roads = self.graph.number_of_edges()

        if num_cities == 0:
            self.log_info("\n📊 No data loaded")
            return

        degrees = [d for n, d in self.graph.degree()]
        avg_degree = sum(degrees) / len(degrees) if degrees else 0

        max_node = max(self.graph.degree(), key=lambda x: x[1])[0]
        max_city = self.cities[max_node]["name"]

        self.log_info(f"\n📊 STATISTICS\n{'='*40}")
        self.log_info(f"Cities: {num_cities}")
        self.log_info(f"Roads: {num_roads}")
        self.log_info(f"Avg Connections: {avg_degree:.2f}")
        self.log_info(f"Most Connected: {max_city}")

        self.status_label.config(text="Statistics displayed")


    def save_to_backend(self):
        """Save to files"""
        try:
            with open("data/cities.txt", "w") as f:
                f.write("CityID,CityName,X_Coord,Y_Coord\n")
                for city_id, data in sorted(self.cities.items()):
                    f.write(f"{city_id},{data['name']},{data['x']},{data['y']}\n")

            with open("data/roads.txt", "w") as f:
                f.write("FromCityID,ToCityID,Distance\n")
                for u, v, data in self.graph.edges(data=True):
                    f.write(f"{u},{v},{data['weight']}\n")

            self.log_info("💾 Saved successfully!")
            messagebox.showinfo("Success", "Graph saved to files!")
            self.status_label.config(text="Saved to files")

        except Exception as e:
            messagebox.showerror("Error", f"Save failed: {e}")
            self.status_label.config(text="Save failed")

    def log_info(self, message):
        """Log to text widget"""
        self.info_text.insert(tk.END, message + "\n")
        self.info_text.see(tk.END)

    def save_log(self, algorithm, source, dest, distance):
        """Save to logs.txt"""
        try:
            with open("logs.txt", "a") as f:
                timestamp = datetime.now().strftime("[%Y-%m-%d %H:%M:%S]")
                src = self.cities[source]["name"]
                dst = self.cities[dest]["name"]
                f.write(f"{timestamp} {algorithm}: {src} -> {dst} ({distance} km)\n")
        except:
            pass


# ==================== MAIN ====================

if __name__ == "__main__":
    root = tk.Tk()
    app = CityNavigationGUI(root)
    root.mainloop()