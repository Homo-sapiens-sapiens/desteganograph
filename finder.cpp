#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>
#include <vector>

using namespace std;

inline uint8_t mod256(int v){return uint8_t((v % 256 + 256) % 256);}

vector<int> str_to_vecint(string inp){
	vector<int> vec;
	int c=0;
	for(int i=0; i<inp.size(); i++){
		if(inp[i]>46 and inp[i]<58){
			c*=10;
			c+=inp[i]-48;
		}
		else{
			vec.push_back(c);
			c=0;
	}}
	return vec;}

struct Photo{
	int w, h, c, siz; vector<uint8_t> data;
	//constuctor by download
	Photo(string name){
		uint8_t* img = stbi_load(name.c_str(), &w, &h, &c, 3);
		if (!img) {cout << "Failed to load image\n"; return;}
		c = 3;
		data.assign(img, img + static_cast<size_t>(w) * h * c);
		siz = data.size();
		stbi_image_free(img);
	}
	//constructor by parameters
	Photo(vector<uint8_t> d, int ww, int hh){
		if(d.size() != ww*hh*3){throw invalid_argument("failed to create Photo object: invalid parameters");}
		data = d;
		w = ww;
		h = hh;
		c = 3;
		siz = ww*hh*3;
	}
	//destructor
	~Photo(){return;}
	//uploader
	void output(string name){
		stbi_write_png(name.c_str(), w, h, 3, data.data(), w * 3);
		cout << "Done\n";}
	//copier
	Photo(const Photo& orig) = default;
	//decrease resolution
	void shrink(int yless, int xless){
		vector<uint8_t> nata;
		int r, g, b;
		for(int y = 0; y < h/yless; y++){for(int x = 0; x < w/xless; x++){
			r=0;g=0;b=0;
			for(int yy=0; yy<yless; yy++){for(int xx=0; xx<xless; xx++){
				r+=data[3*(((y*yless+yy)*w)+(x*xless+xx))];
				g+=data[3*(((y*yless+yy)*w)+(x*xless+xx))+1];
				b+=data[3*(((y*yless+yy)*w)+(x*xless+xx))+2];}}
			nata.push_back(r/(yless*xless));
			nata.push_back(g/(yless*xless));
			nata.push_back(b/(yless*xless));}}
		data=nata;
		w/=xless; h/=yless;
		siz=w*h*3;}
	//find steganographically hidden image using the original
	Photo find(string sz){
		vector<uint8_t> nata;
		vector<int> sizes = str_to_vecint(sz);
		int nw = sizes[0];
		int nh = sizes[1];
		for(int i=0; i<nw*nh*3; i++){nata.push_back((data[i*3]%4)*64+(data[i*3+1]%8)*8+data[i*3+2]%8);}
		return Photo(nata, nw, nh);
	}
};

int main(){
	string carry;
	cout<<"input image name: "; cin>>carry;
	Photo fot = Photo(carry);
	cout<<"input sneaky code: "; cin>>carry;
	Photo fin = fot.find(carry);
	cout<<"input name for result image: "; cin>>carry;
	fin.output(carry);
}