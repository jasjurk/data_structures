import java.util.ArrayList;
import java.util.List;

public class Splay_map<T> 
{	
	private Node<T> root = null;
	
	private class Node<T2>
	{
		public int key = 0;
		public Node<T2> left = null;
		public Node<T2> right = null;
		public List<T2> data = new ArrayList<T2>();
		/*public Node(int key)
		{
			this.key = key;
		}*/
		
		public Node(int key, T2 obj)
		{
			this.key = key;
			data.add(obj);
		}
		
		public Node(int key, List<T2> obj)
		{
			this.key = key;
			for(int i = 0; i < obj.size(); i++) 
				data.add(obj.get(i));
		}
	}
	
	public Splay_map() {}
	
	public Splay_map(Node<T> pointer) 
	{
		if (pointer != null)
		{
			root = new Node<T>(pointer.key, pointer.data);
			copy(pointer.right, root, true);
			copy(pointer.left, root, false);
		}
	}
	
	public Splay_map(Node<T> pointer, Node<T> blocker) 
	{
		if (pointer != null)
		{
			root = new Node<T>(pointer.key, pointer.data);
			if (pointer.right != blocker) 
				copy(pointer.right, root, true);
			if (pointer.left != blocker)
				copy(pointer.left, root, false);
		}
	}
	
	private void copy(Node<T> pointer, Node<T> parent, boolean strona)
	{
		if (pointer != null)
		{
			Node<T> wezel = new Node<T>(pointer.key, pointer.data);
			if (strona) parent.right = wezel;
			else parent.left = wezel;
			if (pointer.right != null) 
				copy(pointer.right, wezel, true);
			if (pointer.left != null) 
				copy(pointer.left, wezel, false);
		}
	}
	
	private void dispose(Node<T> pointer)
	{
		if (pointer.right != null) dispose(pointer.right);
		pointer.right = null;
		if (pointer.left != null) dispose(pointer.left);
		pointer.left = null;
		pointer.data = null;
		pointer.key = 0;
	}
	
	public void dispose()
	{
		dispose(root);
		root = null;
	}
	
	private List<Node<T>> find_path(Node<T> pointer, int get)
	{
		List<Node<T>> wierz = new ArrayList<Node<T>>();
		while (pointer != null)
		{
			wierz.add(pointer);
			if (pointer.key == get) pointer = null;
			else if (pointer.key > get) 
				pointer = pointer.left;
			else
				pointer = pointer.right;
		}
		return wierz;
	}
	
	public List<T> find_list(int key)
	{
		splay(root, key);
		if (root.key == key) 
			return root.data;
		return null;
	}
	
	public T find(int key)
	{
		splay(root, key);
		if (root != null) if (root.key == key) 
			return root.data.get(0);
		return null;
	}
	
	public void update(int key, T data, int index)
	{
		List<Node<T>> wierz = find_path(root, key);
		Node<T> pointer = wierz.get(wierz.size() - 1);
		if (pointer.key == key)
		{
			pointer.data.set(index, data);
		}
	}
	
	public void update(int key, T data) {update(key, data, 0);}
	
	public Splay_map<T> inrange(int min, int max)
	{
		splay(root, min);
		splay(root.right, max);
		return new Splay_map<T>(root.right.left);
	}
	
	
	public Splay_map<T>[] split(int key)
	{
		@SuppressWarnings("unchecked")
		Splay_map<T>[] wynik = new Splay_map[2];
		if (root != null)
		{
			splay(root, key);
			if (root.key == key)
			{
				wynik[0] = new Splay_map<T>(root, root.left);
				wynik[1] = new Splay_map<T>(root.left);
			}
			else if (root.key > key)
			{
				wynik[0] = new Splay_map<T>(root, root.left);
				wynik[1] = new Splay_map<T>(root.left);
			}
			else if (root.key < key)
			{
				wynik[0] = new Splay_map<T>(root.right);
				wynik[1] = new Splay_map<T>(root, root.right);
			}
		}
		return wynik;
	}
	
	public void marge(Splay_map<T> map1, Splay_map<T> map2)
	{System.out.println("GOD LEFT ME UNFINISHED!!!!(merge)");}
	
	private void rot_r(Node<T> pointer, Node<T> parent)
	{
		Node<T> syn = pointer.right;
		if (parent == null) root = syn;
		else if (parent.right == pointer) parent.right = syn;
		else parent.left = syn;
		Node<T> pomoc = syn.left;
		syn.left = pointer;
		pointer.right = pomoc;
	}
	
	private void rot_l(Node<T> pointer, Node<T> parent)
	{
		Node<T> syn = pointer.left;
		if (parent == null) root = syn;
		else if (parent.right == pointer) parent.right = syn;
		else parent.left = syn;
		Node<T> pomoc = syn.right;
		syn.right = pointer;
		pointer.left = pomoc;
	}
	
	private void zigzig(Node<T> pointer, Node<T> child, Node<T> grand, Node<T> parent)
	{
		if (pointer.right == child)
		{
			rot_r(pointer, parent);
			rot_r(child, parent);
		}
		else
		{
			rot_l(pointer, parent);
			rot_l(child, parent);
		}
	}
	
	private void zigzag(Node<T> pointer, Node<T> child, Node<T> grand, Node<T> parent)
	{
		if (pointer.right == child)
		{
			rot_l(child, pointer);
			rot_r(pointer, parent);
		}
		else
		{			
			rot_r(child, pointer);
			rot_l(pointer, parent);

		}
	}

	public void insert(int key, T data)
	{
		if (root == null) {root = new Node<T>(key, data); return;}
		List<Node<T>> wierz = find_path(root, key);
		if (wierz.get(wierz.size() - 1).key == key)
		{wierz.get(wierz.size() - 1).data.add(data);}
		else
		{
			Node<T> pointer = wierz.get(wierz.size() - 1);
			if (pointer.key > key)
			{
				pointer.left = new Node<T>(key, data);
			}
			else
			{
				pointer.right = new Node<T>(key, data);
			}
		}
		splay(root, key);
	}
	
	public void delete(int key)
	{
		if (root == null) return;
		splay(root, key);
		if (root.key == key)
		{
			if (root.right != null) splay(root.right, key);
			if (root.left != null) splay(root.left, key);
			if (root.left != null) 
			{
				root.left.right = root.right;
				root = root.left;
			}
			else
			{
				root = root.right;
			}
		}
	}
	
	private void splay(Node<T> pointer, int key)
	{
		if (root != null)
		{
		List<Node<T>> wierz = find_path(pointer, key);
		int i = wierz.size() - 3;
		Node<T> child;
		Node<T> parent;
		Node<T> x = wierz.get(wierz.size() - 1);
		while (i >= 0)
		{
			pointer = wierz.get(i);
			child = wierz.get(i + 1);
			if (i > 0) parent = wierz.get(i - 1);
			else parent = null;
			if (pointer.right == child)
			{
				if (pointer.right.right == x)
				{
					zigzig(pointer, child, x, parent);
				}
				else
				{
					zigzag(pointer, child, x, parent);
				}
			}
			else
			{
				if (pointer.left.left == x)
				{
					zigzig(pointer, child, x, parent);
				}
				else
				{
					zigzag(pointer, child, x, parent);
				}
			}
			i -= 2;
		}
		if (root != x)
		{
			 if (root.right != null && root.right == x)
			 {
				 rot_r(root, null);
			 }
			 else if (root.left != null && root.left == x) 
				 rot_l(root, null);
		}
		}
	}
	
	private void printtree(Node<T> pointer, int level)
	{
		if(pointer != null)
		{
			System.out.print(pointer.key);
			System.out.print(" ");
			if(pointer.right != null) printtree(pointer.right, level + 1);
			else {System.out.print("| ");}
			if(pointer.left != null) printtree(pointer.left, level + 1);
			else {System.out.print("| ");}
		}
	}
	
	public void printtree() {printtree(root, 0);}
	
	public int max_depth(Node<T> pointer, int depth)
	{
		int maks = depth, temp = 0;
		if(pointer.right != null) temp = max_depth(pointer.right, depth + 1);
		if (temp > maks) maks = temp;
		if(pointer.left != null) temp = max_depth(pointer.left, depth + 1);
		if (temp > maks) maks = temp;
		return maks;
	}
}
