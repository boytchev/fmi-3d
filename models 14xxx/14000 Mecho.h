GLuint wood;

class Rail3D : public Object3D
{
	private:
		Sphere3D *s1;
		Sphere3D *s2;
		Cylinder3D *c;
	public:
		Rail3D( double length );
		void drawImage();
};


Rail3D::Rail3D( double length ) : Object3D()
{
	s1 = new Sphere3D( 1.5 );
	s1->setTexture( wood );
	s1->rotateLocal( 90, {0,1,0} );
	s1->translateLocal( {0,0,length} );
	s1->setTextureSize( 2 );
	s2 = new Sphere3D( 1.5 );
	s2->setTexture( wood );
	s2->rotateLocal( 90, {0,1,0} );
	s2 ->setTextureSize( 2 );
	c = new Cylinder3D( 0.4,length,false,QUALITY_LOW );
	c->setTexture( wood );
	c->setTextureScale( {1,1,0.25,2} );
	c->rotateLocal( 90, {0,1,0} );
	c->setColor( {0.85,0.75,0.65} );
}

void Rail3D::drawImage()
{
	prepareImageProps();
	s1->drawImage();
	s2->drawImage();
	c->drawImage();
	restoreImageProps();
}

class Holder3D : public Object3D
{
	private:
		Cone3D *c1;
		Cone3D *c2;
		Cylinder3D *c;
	public:
		Holder3D( double length );
		void drawImage();
};


Holder3D::Holder3D( double length ) : Object3D()
{
	c1 = new Cone3D( 0.75,0.5,false );
	c1->setCenter( {0,0,length/2} );
	c1->setTexture( wood );
	c2 = new Cone3D( 0.75,0.5,false );
	c2->setCenter( {0,0,-length/2} );
	c2->setTexture( wood );
	c2->rotateLocal( 180, {0,1,0} );
	c = new Cylinder3D( 0.75,length,false );
	c->setTexture( wood );
	c->setCenter( {0,0,-length/2} );
}

void Holder3D::drawImage()
{
	prepareImageProps();
	c1->drawImage();
	c2->drawImage();
	c->drawImage();
	restoreImageProps();
}
