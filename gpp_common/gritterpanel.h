#pragma once

class CFixedReal
{
public:
	enum { DECIMAL = 16 };
	CFixedReal();
	CFixedReal( int i )
	{
		x = i << DECIMAL;
	}
	CFixedReal( const CFixedReal& fx )
	{
		x = fx.x;
	}

	CFixedReal operator+(const CFixedReal& fx) const
	{
		return x + fx.x;
	}

	CFixedReal operator-(const CFixedReal& fx) const
	{
		return x - fx.x;
	}

	CFixedReal operator*(const CFixedReal& fx) const
	{
		return (int)(((__int64)x * (__int64)fx.x)>>DECIMAL*2);
	}

	CFixedReal operator/(const CFixedReal& fx) const
	{
		return (int)((((__int64)x<<DECIMAL) / (__int64)fx.x));
	}

	__forceinline operator int() const
	{
		return x>>DECIMAL;
	}

private:
	int x;
};

/* -------------------------------------------------------------------------------------
 *
 * -------------------------------------------------------------------------------------*/
class CBitImage
{
public:
	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	CBitImage()
	{
		memset( m_image, 0, sizeof(m_image) );
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	virtual ~CBitImage()
	{
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void clear()
	{
		memset( m_image, 0, sizeof(m_image) );
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	bool getPixel( unsigned int x, unsigned int y )
	{
		if( x >= 16 || y >= 16 )
		{
			return false;
		}
		return (m_image[(y*2)+(x/8)] & (1 << (7-(x&7)))) != 0; 
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void setPixel( unsigned int x, unsigned int y, bool bit )
	{
		if( x >= 16 || y >= 16 )
		{
			return;
		}

		if( bit )
		{
			m_image[(y*2)+(x/8)] = m_image[(y*2)+(x/8)] | (1 << (7-(x&7))); 
		}
		else
		{
			m_image[(y*2)+(x/8)] = m_image[(y*2)+(x/8)] & ~(1 << (7-(x&7))); 
		}
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void setLine( int x, int y, int x2, int y2, bool bit )
	{
		CFixedReal ln( max(abs(x2-x), abs(y2-y)) );
		CFixedReal vx = CFixedReal(CFixedReal(x2) - CFixedReal(x)) / ln;
		CFixedReal vy = CFixedReal(CFixedReal(y2) - CFixedReal(y)) / ln;
		CFixedReal cx(x), cy(y);
		for(int i=0; i<ln; i++)
		{
			cx = cx + vx;
			cy = cy + vy;
			setPixel( (int)cx, (int)cy, bit );
		}
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	unsigned char* getImage()
	{
		return &m_image[0];
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void copyTo( unsigned char* pDest, int mode )
	{
		switch( mode )
		{
		case 0:
			memcpy( &pDest[0], &m_image[0], min(sizeof(m_image), (size_t)2*16) );
			break;

		case 1:
			memcpy( &pDest[0], &m_image[0], min(sizeof(m_image), (size_t)2*16/2) );
			break;

		case 2:
			memcpy( &pDest[2*16/2], &m_image[0], min(sizeof(m_image), (size_t)2*16/2) );
			break;
		}
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void copyFrom( unsigned char* pSrc )
	{
		memcpy( &m_image[0], pSrc, sizeof(m_image) );
	}

protected:
	unsigned char m_image[2*16];
};

