from copy import deepcopy

class Node(object):

    GOAL_STATE = [1,2,3,4,5,6,7,8,0]

    def __init__(self, state, parent, action, depth, cost, path_cost):
        self.state = state 
        self.parent = parent
        self.action = action 
        self.depth = depth
        self.cost = cost
        self.path_cost = path_cost
    
    def gen_nodes(self): 
        next_states = []
        zero = self.state.index(0)

        if zero not in [2,5,8]: #last column indexes
            new_state, move_cost = self.swap(zero, 1)
            next_states.append(Node(new_state, self, "left", self.depth + 1, move_cost, self.path_cost + move_cost))

        if zero not in [0,3,6]: #first column indexes
            new_state, move_cost = self.swap(zero, -1)
            next_states.append(Node(new_state, self, "right", self.depth + 1, move_cost, self.path_cost + move_cost))

        if zero not in [6,7,8]: #last row indexes 
            new_state, move_cost = self.swap(zero, 3)
            next_states.append(Node(new_state, self, "up", self.depth + 1, move_cost, self.path_cost + move_cost))

        if zero not in [0,1,2]: #first row indexes
            new_state, move_cost = self.swap(zero, -3)
            next_states.append(Node(new_state, self, "down", self.depth + 1, move_cost, self.path_cost + move_cost))
        
        return next_states

    def swap(self, zero, move): 
        new_state = deepcopy(self.state)
        temp = new_state[zero + move]
        new_state[zero + move] = 0
        new_state[zero] = temp
        return new_state, temp

    def misplaced(self): 
        misplaced = 0
        for i in range(0, len(self.GOAL_STATE) - 1):
            if self.GOAL_STATE[i] != self.state[i]:
                misplaced += 1
        return misplaced 

    def manhattan(self):
        total = 0
        for elem in self.state: 
            elem_index = self.state.index(elem)
            goal_index = self.GOAL_STATE.index(elem)
            man_dist = abs((goal_index // 3) - (elem_index // 3)) \
            + abs((goal_index % 3) - (elem_index % 3))
            total += man_dist
        return total

    def solver(algo, init_state): 
        print("solving...")
        prev_states = [init_state]
        queue = [Node(init_state, None, None, 0, 0, 0)]
        nodes_popped = 0
        max_queue = 0
        while (len(queue) > 0):
            max_queue = max(max_queue, len(queue))
            curr_node = queue.pop()
            nodes_popped += 1
            if curr_node.state == Node.GOAL_STATE:
                print("Goal-state found.")
                return curr_node
            next_states = curr_node.gen_nodes()
            for node in next_states:
                if node.state not in prev_states: 
                    prev_states.append(node.state)
                    #print(node.state)
                    if algo == "BFS":
                        queue.insert(0, node)
                    else:                           ###############################classic DFS###############################
                        queue.append(node)
            if algo == "UCS": 
                queue.sort(key = lambda node: node.path_cost, reverse=True)
            elif algo == "GBF": 
                queue.sort(key = lambda node: node.misplaced(), reverse=True)
            elif algo == "A*1": 
                queue.sort(key = lambda node: node.misplaced() + node.path_cost, reverse=True)
            elif algo == "A*2": 
                queue.sort(key = lambda node: node.manhattan() + node.path_cost, reverse=True)                
        return False
    
def main(): 
    #some alogithms take time to find (several minutes), but they find a solution, just be patient !
    INITIAL_STATE = [8,1,3,4,0,2,7,6,5]
    algo = input("which Search algorithm would you like to use ? \n \
                A) Breath-first-search (tap => BFS)\n \
                B) Depth-first-search (tap => DFS)\n \
                C) Uniform-cost (tap => UCS)\n \
                D) Gredy-Best-First (tap => GBF)\n \
                C) A*1 (missplaced heuristic) (tap => A*1)\n \
                C) A*2(manhatan heuristic) (tap => A*2)\n")
    x = Node.solver(algo , INITIAL_STATE)
    
    for i in range(0,x.depth):
        print(x.state)
        x = x.parent
        # hocam DFS için okadar verimsiz ki printin bittik hali gelmiyor ama çalışiyor. 
        # ayrica, algoritmalarin isimleri caps lock ile yazdığınızdan emin olunuz. 





main()