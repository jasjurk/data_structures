import java.util.ArrayList;
import java.util.List;

public class AVL_map<T> 
{
	private byte[][] nzmn1 = new byte[5][5];
	private byte[][] nzmm1 = new byte[5][5];
	private byte[][] nzmn2 = new byte[5][5];
	private byte[][] nzmm2 = new byte[5][5];
	
	public Node<T> root = null;
	
	private class Node<T2>
	{
		public int key = 0;
		public byte bf = 0;
		public Node<T2> left = null;
		public Node<T2> right = null;
		public List<T2> data = new ArrayList<T2>();
		
		public Node(int key, T2 obj)
		{
			this.key = key;
			data.add(obj);
		}
	}
	
	public AVL_map() 
	{
		for(byte i = -2; i <= 2; i++)
		{
			for(byte i2 = -2; i2 <= 2; i2++)
			{
				byte x = 0, y = (byte) (x-i2), z;
				if (x > y) z = (byte) ((x + 1) - i);
				else z = (byte) ((y + 1) - i);
				nzmn1[i + 2][i2 + 2] = (byte) (y - z);
				if (y > z) nzmm1[i + 2][i2 + 2] = (byte) (x - (y + 1));
				else nzmm1[i + 2][i2 + 2] = (byte) (x - (z + 1));
			}
		}
		for(byte i = -2; i <= 2; i++)
		{
			for(byte i2 = -2; i2 <= 2; i2++)
			{
				byte x = 0, y = (byte) (x+i2), z;
				if (x > y) z = (byte) ((x + 1) + i);
				else z = (byte) ((y + 1) + i);
				nzmn2[i + 2][i2 + 2] = (byte) (z - y);
				if (y > z) nzmm2[i + 2][i2 + 2] = (byte) ((y + 1) - x);
				else nzmm2[i + 2][i2 + 2] = (byte) ((z + 1) - x);
			}
		}
	}
	
	public void dispose(Node<T> pointer)
	{
		if (pointer.right != null) dispose(pointer.right);
		pointer.right = null;
		if (pointer.left != null) dispose(pointer.left);
		pointer.left = null;
	}
	
	public void dispose()
	{
		dispose(root);
		root = null;
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
	
	public List<Node<T>> find_path(Node<T> pointer, int get)
	{
		List<Node<T>> wierz = new ArrayList<Node<T>>();
		while (pointer != null)
		{
			wierz.add(pointer);
			if (pointer.key == get) {pointer = null;}
			else if (pointer.key > get) 
				pointer = pointer.left;
			else
				pointer = pointer.right;
		}
		return wierz;
	}
	
	public List<Node<T>> find_path_min(Node<T> pointer)
	{
		List<Node<T>> wierz = new ArrayList<Node<T>>();
		while (pointer != null) 
		{wierz.add(pointer); pointer = pointer.left;}
		return wierz;
	}
	
	public List<Node<T>> find_path_max(Node<T> pointer)
	{
		List<Node<T>> wierz = new ArrayList<Node<T>>();
		while (pointer != null) 
		{wierz.add(pointer); pointer = pointer.right;}
		return wierz;
	}
	
	public T max()
	{
		List<Node<T>> wierz = find_path_max(root);
		if (root == null) return null;
		return wierz.get(wierz.size() - 1).data.get(0);
	}
	
	public int max_ind()
	{
		List<Node<T>> wierz = find_path_max(root);
		if (root == null) return -2100000000;
		return wierz.get(wierz.size() - 1).key;
	}
	
	public T min()
	{
		List<Node<T>> wierz = find_path_min(root);
		if (root == null) return null;
		return wierz.get(wierz.size() - 1).data.get(0);
	}
	
	public int min_ind()
	{
		List<Node<T>> wierz = find_path_min(root);
		if (root == null) return 2100000000;
		return wierz.get(wierz.size() - 1).key;
	}
	
	public List<T> find_list(int key)
	{
		List<Node<T>> wierz = find_path(root, key);
		if (wierz.get(wierz.size() - 1).key == key) 
			return wierz.get(wierz.size() - 1).data;
		return null;
	}
	
	public T find(int key)
	{
		List<Node<T>> wierz = find_path(root, key);
		if (root != null && wierz.get(wierz.size() - 1).key == key) 
			return wierz.get(wierz.size() - 1).data.get(0);
		return null;
	}
	
	public void rot_r(Node<T> pointer, Node<T> parent)
	{
		Node<T> syn = pointer.right;
		int n = pointer.bf, m = syn.bf;
		pointer.bf = nzmn1[n + 2][m + 2];
		syn.bf = nzmm1[n + 2][m + 2];
		Node<T> pomoc = syn.left;
		syn.left = pointer;
		pointer.right = pomoc;
		if (parent == null) root = syn;
		else if (parent.right == pointer) parent.right = syn;
		else parent.left = syn;
	}
	
	public void rot_l(Node<T> pointer, Node<T> parent)
	{
		Node<T> syn = pointer.left;
		int n = pointer.bf, m = syn.bf;
		pointer.bf = nzmn2[n + 2][m + 2];
		syn.bf = nzmm2[n + 2][m + 2];
		Node<T> pomoc = syn.right;
		syn.right = pointer;
		pointer.left = pomoc;
		if (parent == null) root = syn;
		else if (parent.right == pointer) parent.right = syn;
		else parent.left = syn;
	}
	
	public int rebalance(Node<T> pointer, Node<T> parent)
	{
		int zwroc = 0;
		if (pointer.bf < -1 || pointer.bf > 1)
		{
			if (pointer.bf == 2)
			{
				if (pointer.right.bf == -1)
				{
					zwroc = 1;
					rot_l(pointer.right, pointer);
					rot_r(pointer, parent);
				}
				else
				{
					if (pointer.right.bf == 0) zwroc = 2;
					if (pointer.right.bf == 1) zwroc = 3;
					rot_r(pointer, parent);
				}
			}
			else if (pointer.bf == -2)
			{
				if (pointer.left.bf == 1)
				{
					zwroc = 4;
					rot_r(pointer.left, pointer);
					rot_l(pointer, parent);
				}
				else
				{
					if (pointer.left.bf == 0) zwroc = 5;
					if (pointer.left.bf == -1) zwroc = 6;
					rot_l(pointer, parent);
				}
			}
		}
		return zwroc;
	}

	public void insert(int key, T data)
	{
		if (root == null) {root = new Node<T>(key, data); return;}
		List<Node<T>> wierz = find_path(root, key);
		Node<T> parent;
		if (wierz.get(wierz.size() - 1).key == key)
		{wierz.get(wierz.size() - 1).data.add(data); return;}
		else
		{
			Node<T> pointer = wierz.get(wierz.size() - 1);
			if (pointer.key > key)
			{
				pointer.left = new Node<T>(key, data);
				wierz.add(pointer.left);
			}
			else
			{
				pointer.right = new Node<T>(key, data);
				wierz.add(pointer.right);
			}
			int i = wierz.size() - 2;
			while (i >= 0)
			{
				pointer = wierz.get(i);
				if (i > 0) parent = wierz.get(i - 1);
				else parent = null;
				if (wierz.get(i + 1) == pointer.left) pointer.bf --;
				else pointer.bf ++;
				if (pointer.bf == 0) return;
				int bal = rebalance(pointer, parent);
				if (bal != 0) return;
				i--;
			}
		}
	}
	
	public void del_leaf(Node<T> pointer, Node<T> parent)
	{
		if (pointer.left == null || pointer.right == null)
		{
			if (pointer.right != null) 
			{
				pointer.right.bf = 0;
				if (parent == null)
				{
					root = pointer.right;
				}
				else
				{
					if(parent.right == pointer) 
						parent.right = pointer.right;
					else parent.left = pointer.right;
				}
			}
			else if (pointer.left != null) 
			{
				pointer.left.bf = 0;
				if (parent == null)
				{
					root = pointer.left;
				}
				else
				{
					if(parent.right == pointer) 
						parent.right = pointer.left;
					else parent.left = pointer.left;
				}
			}
			else
			{
				if (parent == null) root = null;
				else if(parent.right == pointer) 
					parent.right = null;
				else parent.left = null;
			}
		}
		pointer.data = null;
		pointer.right = null;
		pointer.left = null;
	}
	
	public void delete(int key)
	{
		List<Node<T>> wierz = find_path(root, key);
		int i = wierz.size() - 1;
		if (i >= 0)
		{
		Node<T> pointer = wierz.get(i);
		Node<T> parent;
		Node<T> delete = pointer;
		if (pointer.key != key) return;
		if (pointer.left != null && pointer.right != null)
			wierz.addAll(find_path_max(pointer.left));
		i = wierz.size() - 1;
		wierz.get(i).bf = 0;
		//pointer.bf = 0;
		i--;
		while(i >= 0)
		{
			pointer = wierz.get(i);
			if (i > 0) parent = wierz.get(i - 1);
			else parent = null;
			if (wierz.get(i + 1).key < wierz.get(i).key) 
				pointer.bf ++;
			else pointer.bf --;
			if (pointer.bf == 1 || pointer.bf == -1) i = 0;
			else
			{
				int rot = rebalance(pointer, parent);
				if (rot == 2 || rot == 5) i = 0;
			}
			i--;
		}
		i = wierz.size() - 1;
		delete.key = wierz.get(i).key;
		delete.data = wierz.get(i).data;
		if (i > 0) del_leaf(wierz.get(i), wierz.get(i - 1));
		else del_leaf(wierz.get(i), null);
		}
	}
	
	public void printtree(Node<T> pointer, int level)
	{
		if(pointer != null)
		{
			System.out.print(pointer.key);
			System.out.print(":");
			System.out.print(pointer.bf);
			System.out.print(" ");
			if(pointer.right != null) printtree(pointer.right, level + 1);
			else {System.out.print("| ");}
			if(pointer.left != null) printtree(pointer.left, level + 1);
			else {System.out.print("| ");}
		}
	}
}
