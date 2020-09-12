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
	s1 = new Sphere3D( 0.75,QUALITY_LOW );
	s1->setCenter( {length,0,0} );
	s1->setTexture( wood );
	s1->rotateLocal( 90, {0,1,0} );
	s2 = new Sphere3D( 0.75,QUALITY_LOW );
	s2->setTexture( wood );
	s2->rotateLocal( 90, {0,1,0} );
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
	c1 = new Cone3D( 0.75,0.5,false,QUALITY_LOW );
	c1->setCenter( {0,0,length/2} );
	c1->setTexture( wood );
	c2 = new Cone3D( 0.75,0.5,false,QUALITY_LOW );
	c2->setCenter( {0,0,-length/2} );
	c2->setTexture( wood );
	c2->rotateLocal( 180, {0,1,0} );
	c = new Cylinder3D( 0.75,length,false,QUALITY_LOW );
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

class Pencil3D : public Object3D
{
	private:
		Cylinder3D *cy[4];
		Cone3D *co[2];
	public:
		Pencil3D( double length );
		void drawImage();
};


Pencil3D::Pencil3D( double length ) : Object3D()
{
	cy[0] = new Cylinder3D( 0.3,length-2.5,false,QUALITY_LOW );
	cy[0]->setCenter( {0,0,1.5} );
	cy[0]->setColor( {0.8,0.3,0} );

	cy[1] = new Cylinder3D( 0.3,0.2,false,QUALITY_LOW );
	cy[1]->setCenter( {0,0,length-1} );
	cy[1]->setColor( {1,1,1} );

	cy[2] = new Cylinder3D( 0.3,0.8,false,QUALITY_LOW );
	cy[2]->setCenter( {0,0,length-0.8} );
	cy[2]->setColor( {0.5,0.1,0} );

	cy[3] = new Cylinder3D( 0.05,1,false,QUALITY_LOW );
	cy[3]->setCenter( {0,0,1} );
	cy[3]->setColor( {0,0,0} );
	cy[3]->rotateLocal( 180, {0,1,0} );

	co[0] = new Cone3D( 0.3,1.5,false,QUALITY_LOW );
	co[0]->setCenter( {0,0,1.5} );
	co[0]->setColor( {1,0.9,0.7} );
	co[0]->rotateLocal( 180, {0,1,0} );

	co[1] = new Cone3D( 0.3,0.05,false,QUALITY_LOW );
	co[1]->setCenter( {0,0,length} );
	co[1]->setColor( {0.6,0.3,0} );
}

void Pencil3D::drawImage()
{
	prepareImageProps();
	for( int i=0; i<4; i++ ) cy[i]->drawImage();
	for( int i=0; i<2; i++ ) co[i]->drawImage();
	restoreImageProps();
}

