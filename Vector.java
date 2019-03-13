
public class Vector
{
	int capacity = 1;
	int used = 0;
	double wsp = 2;
	public Fastint[] data = new Fastint[1];
	public Vector() {}
	public Vector(int n) 
	{
		relocate(n);
	}
	
	public void relocate(int n)
	{
		if (n > capacity)
		{
			Fastint[] temp = new Fastint[n];
			for(int i = 0;i < used; i++)
				temp[i] = data[i];
			data = temp;
			capacity = n;
		}
	}
	
	public void push_back(Fastint dane)
	{
		if (used >= capacity)
			relocate((int)Math.ceil(capacity * wsp));
		data[used] = dane;
		used++;
	}
	
	public Fastint get(int index) {return data[index];}
	
	public void set(int index, Fastint dane) {data[index] = dane;}
	
	public int n() {return used;}
}
