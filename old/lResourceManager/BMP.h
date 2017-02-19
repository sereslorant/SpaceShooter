#ifndef BMP_H
#define BMP_H

#include <inttypes.h>

#include <string>
#include <fstream>

#include <iostream>

int16_t Cmb_sInt2b_LE(char *Src);
uint16_t Cmb_uInt16_LE(char *Src);

int32_t Cmb_sInt32_LE(char *Src);
uint32_t Cmb_uInt32_LE(char *Src);

class BMP
{
public:
    enum ERROR
    {
        ERR_OK                      = 0,
        ERR_COULDNT_OPEN_FILE       = 1,
        ERR_FILE_NOT_BMP            = 2,
        ERR_DIB_HEADER_UNSUPPORTED  = 4,
        ERR_UNSUPPORTED_DEPTH       = 8,
        ERR_UNSUPPORTED_COMPRESSION = 16,
        ERR_UNSUPPORTED_BITMASK     = 32,
    };

    enum DEPTH
    {
        D_0 = 0,
        D_1 = 1,
        D_4 = 4,
        D_8 = 8,
        D_16 = 16,
        D_24 = 24,
        D_32 = 32,
        D_UNSUPPORTED = 256,
    };

    enum PIXEL_FORMAT
    {
        PF_R8_G8_B8 = -3,
        PF_B8_G8_R8 = -2,
        PF_LUT = -1,
        //16
        //5_6_5
        PF_B5_G6_R5 = 0,
        PF_R5_G6_B5,
        //4_4_4_4
        PF_A4_B4_G4_R4,
        PF_A4_R4_G4_B4,
        //1_5_5_5
        PF_B5_G5_R5_A1,
        PF_B5_G5_R5_X1,
        PF_R5_G5_B5_A1,
        PF_R5_G5_B5_X1,
        //32
        //2_10_10_10
        PF_A2_B10_G10_R10,
        PF_A2_R10_G10_B10,
        //8_8_8_8
        PF_A8_B8_G8_R8,
        PF_X8_B8_G8_R8,
        PF_A8_R8_G8_B8,
        PF_X8_R8_G8_B8,
    };
private:
    enum DIB_HEADER_TYPE
    {
        BITMAPCOREHEADER	= 12,
        BITMAPCOREHEADER2	= 64,
        BITMAPINFOHEADER	= 40,
        BITMAPV2INFOHEADER	= 52,
        BITMAPV3INFOHEADER	= 56,
        BITMAPV4HEADER		= 108,
        BITMAPV5HEADER		= 124,
        UNSUPPORTED_HEADER  = -1,
    };

    static const uint32_t RED_ARRAY  [];
    static const uint32_t GREEN_ARRAY[];
    static const uint32_t BLUE_ARRAY [];
    static const uint32_t ALPHA_ARRAY[];

    bool DetermineBitmask(uint32_t rMask,uint32_t gMask,uint32_t bMask,uint32_t aMask,PIXEL_FORMAT PixelFormat)
    {
        return  (RED_ARRAY  [PixelFormat]   == rMask)&&
                (GREEN_ARRAY[PixelFormat]   == gMask)&&
                (BLUE_ARRAY [PixelFormat]   == bMask)&&
                (ALPHA_ARRAY[PixelFormat]   == aMask);
    }

    class DIB_Header
    {
    public:
        virtual void Print(std::ostream &out) = 0;

        virtual uint32_t    GetHeaderSize() = 0;
        virtual int32_t     GetWidth()      = 0;
        virtual int32_t     GetHeight()     = 0;
        virtual uint16_t    GetDepth()      = 0;

        virtual bool CompressionAvailable()
        {
            return false;
        }
        virtual uint32_t GetCompression()
        {
            return CM_NOT_AVAILABLE;
        }

        virtual unsigned int GetColorPalette() = 0;

        virtual bool BitmasksAvailable()
        {
            return false;
        }
        virtual uint32_t GetRedMask()
        {
            return 0x00000000;
        }
        virtual uint32_t GetGreenMask()
        {
            return 0x00000000;
        }
        virtual uint32_t GetBlueMask()
        {
            return 0x00000000;
        }
        virtual uint32_t GetAlphaMask()
        {
            return 0x00000000;
        }

        DIB_Header()
        {

        }
        virtual ~DIB_Header()
        {

        }
    };

    class Bitmapcoreheader : public DIB_Header
    {
    protected:
        uint32_t	HeaderSize;
        int32_t	    Width;
        int32_t	    Height;
        uint16_t	ColorPlanes;
        uint16_t	BitsPerPixel;

    public:
        virtual void Print(std::ostream &out) override
        {
            out << "Header size: "      << HeaderSize << "\n";
            out << "Width: "            << Width << "\n";
            out << "Height: "           << Height << "\n";
            out << "Color planes: "     << ColorPlanes << "\n";
            out << "Bits per pixel: "   << BitsPerPixel << "\n";
        }

        virtual uint32_t GetHeaderSize() final override
        {
            return HeaderSize;
        }

        virtual int32_t GetWidth()   final override
        {
            return Width;
        }

        virtual int32_t GetHeight()  final override
        {
            return Height;
        }

        virtual uint16_t GetDepth()final override
        {
            return BitsPerPixel;
        }

        virtual unsigned int GetColorPalette() override
        {
            if(BitsPerPixel == D_1)
            {
                return 2;
            }
            if(BitsPerPixel == D_4)
            {
                return 16;
            }
            if(BitsPerPixel == D_8)
            {
                return 256;
            }

            return 0;
        }

        Bitmapcoreheader(std::istream &in)
        {
            HeaderSize = BITMAPCOREHEADER;
            char Tmp[4];

            in.read(Tmp,4*sizeof(char));
            Width = Cmb_sInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            Height = Cmb_sInt32_LE(Tmp);

            in.read(Tmp,2*sizeof(char));
            ColorPlanes = Cmb_uInt16_LE(Tmp);

            in.read(Tmp,2*sizeof(char));
            BitsPerPixel = Cmb_uInt16_LE(Tmp);
        }
        virtual ~Bitmapcoreheader()
        {

        }
    };

    class Bitmapinfoheader : public Bitmapcoreheader
    {
    protected:
        uint32_t    Compression;
        uint32_t    ImageSize;
        int32_t     HRes;
        int32_t     VRes;
        uint32_t    ColorPalette;
        uint32_t    ImportantColors;
        //Optional bitmasks
        uint32_t oRedMask;
        uint32_t oGreenMask;
        uint32_t oBlueMask;

    public:
        virtual void Print(std::ostream &out) override
        {
            Bitmapcoreheader::Print(out);
            out << "Compression: "      << Compression << "\n";
            out << "Image size: "       << ImageSize << "\n";
            out << "HRes: "             << HRes << "\n";
            out << "VRes: "             << VRes << "\n";
            out << "Color palette: "    << ColorPalette << "\n";
            out << "Important colors: " << ImportantColors << "\n";
        }

        virtual bool CompressionAvailable() final override
        {
            return true;
        }

        virtual uint32_t GetCompression() final override
        {
            return Compression;
        }

        virtual unsigned int GetColorPalette() override
        {
            if(ColorPalette == 0)
            {
                if(BitsPerPixel == D_1)
                {
                    return 2;
                }
                if(BitsPerPixel == D_4)
                {
                    return 16;
                }
                if(BitsPerPixel == D_8)
                {
                    return 256;
                }

                return 0;
            }
            else
            {
                return ColorPalette;
            }
        }

        virtual bool BitmasksAvailable() final override
        {
            if(Compression == CM_BITFIELDS)
            {
                return true;
            }

            return false;
        }

        virtual uint32_t GetRedMask() override
        {
            if(Compression == CM_BITFIELDS)
            {
                return oRedMask;
            }

            return 0x00000000;
        }

        virtual uint32_t GetGreenMask() override
        {
            if(Compression == CM_BITFIELDS)
            {
                return oGreenMask;
            }

            return 0x00000000;
        }

        virtual uint32_t GetBlueMask() override
        {
            if(Compression == CM_BITFIELDS)
            {
                return oBlueMask;
            }

            return 0x00000000;
        }

        Bitmapinfoheader(std::istream &in):Bitmapcoreheader(in)
        {
            HeaderSize = BITMAPINFOHEADER;
            char Tmp[4];

            in.read(Tmp,4*sizeof(char));
            Compression = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ImageSize = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            HRes = Cmb_sInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            HRes = Cmb_sInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ColorPalette = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ImportantColors = Cmb_uInt32_LE(Tmp);

            oRedMask = 0;
            oGreenMask = 0;
            oBlueMask = 0;

            if(Compression == CM_BITFIELDS)
            {
                in.read(Tmp,4*sizeof(char));
                oRedMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];

                in.read(Tmp,4*sizeof(char));
                oGreenMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];

                in.read(Tmp,4*sizeof(char));
                oBlueMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];
            }
        }
        virtual ~Bitmapinfoheader()
        {

        }
    };

    class Bitmapcoreheader2 : public Bitmapinfoheader
    {
    protected:
        uint16_t ResUnit;
        uint16_t Reserved;
        uint16_t Orientation;
        uint16_t Halftoning;
        uint32_t HalftoneSize_1;
        uint32_t HalftoneSize_2;
        uint32_t ColorSpace;
        uint32_t AppData;

    public:
        virtual void Print(std::ostream &out) override
        {
            Bitmapinfoheader::Print(out);
            out << "Compression: "      << Compression << "\n";
            out << "Reserved: "         << Reserved << "\n";
            out << "Orientation: "      << Orientation << "\n";
            out << "Halftoning: "       << Halftoning << "\n";
            out << "HalftoneSize_1: "   << HalftoneSize_1 << "\n";
            out << "HalftoneSize_2: "   << HalftoneSize_2 << "\n";
            out << "Color space: "      << ColorSpace << "\n";
            out << "App data: "         << AppData << "\n";
        }

        Bitmapcoreheader2(std::istream &in):Bitmapinfoheader(in)
        {
            HeaderSize = BITMAPCOREHEADER2;
            char Tmp[4];

            in.read(Tmp,2*sizeof(char));
            ResUnit = Cmb_uInt16_LE(Tmp);

            in.read(Tmp,2*sizeof(char));
            Reserved = Cmb_uInt16_LE(Tmp);

            in.read(Tmp,2*sizeof(char));
            Orientation = Cmb_uInt16_LE(Tmp);

            in.read(Tmp,2*sizeof(char));
            Halftoning = Cmb_uInt16_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            HalftoneSize_1 = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            HalftoneSize_2 = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ColorSpace = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            AppData = Cmb_uInt32_LE(Tmp);
        }
        ~Bitmapcoreheader2()
        {

        }
    };

    class BitmapV2infoheader : public Bitmapinfoheader
    {
    protected:

    public:
        virtual void Print(std::ostream &out) override
        {
            Bitmapinfoheader::Print(out);
            out << "RedMask: "      << std::hex << oRedMask << std::dec << "\n";
            out << "GreenMask: "    << std::hex << oGreenMask << std::dec << "\n";
            out << "BlueMask: "     << std::hex << oBlueMask << std::dec << "\n";
        }

        virtual uint32_t GetRedMask()  final override
        {
            return oRedMask;
        }

        virtual uint32_t GetGreenMask()final override
        {
            return oGreenMask;
        }

        virtual uint32_t GetBlueMask() final override
        {
            return oBlueMask;
        }

        BitmapV2infoheader(std::istream &in):Bitmapinfoheader(in)
        {
            HeaderSize = BITMAPV2INFOHEADER;
            char Tmp[4];

            if(Compression != CM_BITFIELDS)
            {
                in.read(Tmp,4*sizeof(char));
                oRedMask = 0;
                oRedMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];

                in.read(Tmp,4*sizeof(char));
                oGreenMask = 0;
                oGreenMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];

                in.read(Tmp,4*sizeof(char));
                oBlueMask = 0;
                oBlueMask = (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];
            }
        }
        virtual ~BitmapV2infoheader()
        {

        }
    };

    class BitmapV3infoheader : public BitmapV2infoheader
    {
    protected:
        uint32_t AlphaMask;

    public:
        virtual void Print(std::ostream &out) override
        {
            BitmapV2infoheader::Print(out);
            out << "AlphaMask: " << std::hex << AlphaMask << std::dec << "\n";
        }

        virtual uint32_t GetAlphaMask() final override
        {
            return AlphaMask;
        }

        BitmapV3infoheader(std::istream &in):BitmapV2infoheader(in)
        {
            HeaderSize = BITMAPV3INFOHEADER;
            char Tmp[4];

            in.read(Tmp,4*sizeof(char));
            AlphaMask = 0;
            AlphaMask |= (unsigned char)Tmp[0] << 24 | (unsigned char)Tmp[1] << 16 | (unsigned char)Tmp[2] << 8 | (unsigned char)Tmp[3];
        }
        virtual ~BitmapV3infoheader()
        {

        }
    };

    class BitmapV4header : public BitmapV3infoheader
    {
    public:
        struct sInt32Vector
        {
            uint32_t X;
            uint32_t Y;
            uint32_t Z;
        };

        struct sInt32VectorTriple
        {
            sInt32Vector R;
            sInt32Vector G;
            sInt32Vector B;
        };

    protected:
        uint32_t  ColorSpace;
        sInt32VectorTriple Endpoints;
        uint32_t  GammaRed;
        uint32_t  GammaGreen;
        uint32_t  GammaBlue;

    public:
        virtual void Print(std::ostream &out) override
        {
            BitmapV3infoheader::Print(out);
            out << "Color space: " << ColorSpace << "\n";

            out << "Endpoints:\n"
                << "Red:\t("    << Endpoints.R.X << "," << Endpoints.R.Y << "," << Endpoints.R.Z << ")\n";
            out << "Green:\t("  << Endpoints.G.X << "," << Endpoints.G.Y << "," << Endpoints.G.Z << ")\n";
            out << "Blue:\t("   << Endpoints.B.X << "," << Endpoints.B.Y << "," << Endpoints.B.Z << ")\n";

            out << "Gamma red: "    << GammaRed << "\n";
            out << "Gamma green: "  << GammaGreen << "\n";
            out << "Gamma blue: "   << GammaBlue << "\n";
        }

        BitmapV4header(std::istream &in):BitmapV3infoheader(in)
        {
            HeaderSize = BITMAPV4HEADER;
            char Tmp[4];

            in.read(Tmp,4*sizeof(char));
            ColorSpace = Cmb_uInt32_LE(Tmp);

            signed long int x,y,z;

            //Red
            in.read(Tmp,4*sizeof(char));
            x = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            y = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            z = Cmb_uInt32_LE(Tmp);

            sInt32Vector r;
            r.X = x;
            r.Y = y;
            r.Z = z;

            //Green
            in.read(Tmp,4*sizeof(char));
            x = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            y = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            z = Cmb_uInt32_LE(Tmp);

            sInt32Vector g;
            g.X = x;
            g.Y = y;
            g.Z = z;

            //Blue
            in.read(Tmp,4*sizeof(char));
            x = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            y = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            z = Cmb_uInt32_LE(Tmp);

            sInt32Vector b;
            b.X = x;
            b.Y = y;
            b.Z = z;

            Endpoints.R = r;
            Endpoints.G = g;
            Endpoints.B = b;

            in.read(Tmp,4*sizeof(char));
            GammaRed = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            GammaGreen = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            GammaBlue = Cmb_uInt32_LE(Tmp);
        }
        virtual ~BitmapV4header()
        {

        }
    };

    class BitmapV5header : public BitmapV4header
    {
    protected:
        uint32_t Intent;
        uint32_t ProfileData;
        uint32_t ProfileSize;
        uint32_t Reserved;

    public:
        virtual void Print(std::ostream &out) override
        {
            BitmapV4header::Print(out);
            out << "Intent: "       << Intent << "\n";
            out << "Profile data: " << ProfileData << "\n";
            out << "Profile size: " << ProfileSize << "\n";
            out << "Reserved: "     << Reserved << "\n";
        }
        BitmapV5header(std::istream &in):BitmapV4header(in)
        {
            HeaderSize = BITMAPV5HEADER;
            char Tmp[4];

            in.read(Tmp,4*sizeof(char));
            Intent = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ProfileData = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            ProfileSize = Cmb_uInt32_LE(Tmp);

            in.read(Tmp,4*sizeof(char));
            Reserved = Cmb_uInt32_LE(Tmp);
        }
        ~BitmapV5header(){}
    };
    enum COMPRESSION_METHOD
    {
        CM_RGB = 0,
        CM_RLE8 = 1, //Run Length Encoding ha a BitsPerSample == 8
        CM_RLE4 = 2, //Run Length Encoding ha a BitsPerSample == 4
        CM_BITFIELDS = 3,
        CM_JPEG = 4,
        CM_PNG = 5,
        CM_ALPHABITFIELDS = 6,
        CM_NOT_AVAILABLE = 128,
        CM_UNSUPPORTED = 129,
    };

    std::string filename;
    DIB_HEADER_TYPE DIBHeaderType;

    DIB_Header *DIBHeader;
    COMPRESSION_METHOD  CompressionMethod;

    class BMColorTable
    {
    public:
        struct ColorEntry
        {
            uint8_t Alpha;
            uint8_t Blue;
            uint8_t Green;
            uint8_t Red;

            ColorEntry()
                :Alpha(255),Blue(255),Green(255),Red(255)
            {

            }
        };
    private:
        unsigned int nColorEntries;
        ColorEntry *Table;
    public:
        unsigned int Size()
        {
            return nColorEntries;
        }

        ColorEntry &operator[](unsigned int i)
        {
            return Table[i];
        }

        const ColorEntry &operator[](unsigned int i) const
        {
            return Table[i];
        }

        BMColorTable(unsigned int n_colorEntries):nColorEntries(n_colorEntries)
        {
            Table = new ColorEntry[nColorEntries];
        }
        ~BMColorTable()
        {
            delete [] Table;
        }
    };
    int Error;

    int32_t         Width;
    int32_t         Height;
    DEPTH           Depth;
    PIXEL_FORMAT    PixelFormat;
    //  FORMAT      Format;
    //  BITMASK     Bitmask;

    BMColorTable *ColorTable;

    char *PixelArray;

    bool CheckBmp(char *Data)
    {
        return ((Data[0] == 'B' && Data[1] == 'M') || (Data[0] == 'B' && Data[1] == 'A') || (Data[0] == 'C' && Data[1] == 'I') || (Data[0] == 'C' && Data[1] == 'P') || (Data[0] == 'I' && Data[1] == 'C') || (Data[0] == 'P' && Data[1] == 'T'));
    }

    void SetDepth(unsigned int depth)
    {
        if(depth == D_0)
        {
            Depth = D_0;
        }
        else if(depth == D_1)
        {
            Depth = D_1;
        }
        else if(depth == D_4)
        {
            Depth = D_4;
        }
        else if(depth == D_8)
        {
            Depth = D_8;
        }
        else if(depth == D_16)
        {
            Depth = D_16;
        }
        else if(depth == D_24)
        {
            Depth = D_24;
        }
        else if(depth == D_32)
        {
            Depth = D_32;
        }
        else
        {
            Depth = D_UNSUPPORTED;
            Error |= ERR_UNSUPPORTED_DEPTH;
        }
    }

    void SetCompression(unsigned long int compression)
    {
        if(compression == CM_NOT_AVAILABLE)
        {
            CompressionMethod = CM_NOT_AVAILABLE;
        }
        if(compression == CM_RGB)
        {
            CompressionMethod = CM_RGB;
        }
        else if(compression == CM_RLE8)
        {
            CompressionMethod = CM_RLE8;
        }
        else if(compression == CM_RLE4)
        {
            CompressionMethod = CM_RLE4;
        }
        else if(compression == CM_BITFIELDS)
        {
            CompressionMethod = CM_BITFIELDS;
        }
        else if(compression == CM_JPEG)
        {
            CompressionMethod = CM_JPEG;
        }
        else if(compression == CM_PNG)
        {
            CompressionMethod = CM_PNG;
        }
        else if(compression == CM_ALPHABITFIELDS)
        {
            CompressionMethod = CM_ALPHABITFIELDS;
        }
        else
        {
            CompressionMethod = CM_UNSUPPORTED;
            Error |= ERR_UNSUPPORTED_COMPRESSION;
        }
    }

    void SetBitmask(unsigned long int iRedMask,unsigned long int iGreenMask,unsigned long int iBlueMask,unsigned long int iAlphaMask)
    {
        if(DIBHeader->BitmasksAvailable())
        {
            if(Depth == D_16)
            {
                //5_6_5
                if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_B5_G6_R5))
                {
                    //Bitmask = BM_5_6_5;
                    //Format  = F_BGR;
                    PixelFormat = PF_B5_G6_R5;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_R5_G6_B5))
                {
                    //Bitmask = BM_5_6_5;
                    //Format  = F_RGB;
                    PixelFormat = PF_R5_G6_B5;
                }
                //4_4_4_4
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A4_B4_G4_R4))
                {
                    //Bitmask = BM_4_4_4_4;
                    //Format  = F_ABGR;
                    PixelFormat = PF_A4_B4_G4_R4;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A4_R4_G4_B4))
                {
                    //Bitmask = BM_4_4_4_4;
                    //Format  = F_ARGB;
                    PixelFormat = PF_A4_R4_G4_B4;
                }
                //1_5_5_5
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_B5_G5_R5_A1))
                {
                    //Bitmask = BM_1_5_5_5;
                    //Format  = F_ARGB;
                    PixelFormat = PF_B5_G5_R5_A1;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_B5_G5_R5_X1))
                {
                    //Bitmask = BM_1_5_5_5;
                    //Format  = F_ARGB;
                    PixelFormat = PF_B5_G5_R5_X1;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_R5_G5_B5_A1))
                {
                    //Bitmask = BM_1_5_5_5;
                    //Format  = F_ABGR;
                    PixelFormat = PF_R5_G5_B5_A1;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_R5_G5_B5_X1))
                {
                    //Bitmask = BM_1_5_5_5;
                    //Format  = F_XRGB;
                    PixelFormat = PF_R5_G5_B5_X1;
                }
                else
                {
                    Error |= ERR_UNSUPPORTED_BITMASK;
                }
            }
            if(Depth == D_32)
            {
                //2_10_10_10 !!!CSAK OPENGL 3.0-VAL ÉS A FÖLÖTT MŰXIK GARANTÁLTAN!!!
                if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A2_B10_G10_R10))
                {
                    //Bitmask = BM_2_10_10_10;
                    //Format  = F_ABGR;
                    PixelFormat = PF_A2_B10_G10_R10;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A2_R10_G10_B10))
                {
                    //Bitmask = BM_2_10_10_10;
                    //Format  = F_ARGB;
                    PixelFormat = PF_A2_R10_G10_B10;
                }
                //8_8_8_8
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A8_B8_G8_R8))
                {
                    //Bitmask = BM_8_8_8_8;
                    //Format  = F_ABGR;
                    PixelFormat = PF_A8_B8_G8_R8;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_X8_B8_G8_R8))
                {
                    //Bitmask = BM_8_8_8_8;
                    //Format  = F_XBGR;
                    PixelFormat = PF_X8_B8_G8_R8;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_A8_R8_G8_B8))
                {
                    //Bitmask = BM_2_10_10_10;
                    //Format  = F_ARGB;
                    PixelFormat = PF_A8_R8_G8_B8;
                }
                else if(DetermineBitmask(iRedMask,iGreenMask,iBlueMask,iAlphaMask,PF_X8_R8_G8_B8))
                {
                    //Bitmask = BM_8_8_8_8;
                    //Format  = F_XRGB;
                    PixelFormat = PF_X8_R8_G8_B8;
                }
                else
                {
                    Error |= ERR_UNSUPPORTED_BITMASK;
                }
            }

            return;
        }
        if(Depth == D_0)
        {
            //PixelFormat = PF_COMPRESSED
        }
        if(Depth == D_1 || Depth == D_4 || Depth == D_8)
        {
            PixelFormat = PF_LUT;
        }
        if(CompressionMethod == CM_RGB)
        {
            if(Depth == D_16)
            {
                //Bitmask = BM_1_5_5_5;
                //Format = F_XRGB;
                PixelFormat = PF_B5_G5_R5_X1;
            }
            if(Depth == D_24)
            {
                //Bitmask = BM_8_8_8;
                //Format = F_RGB;
                PixelFormat = PF_R8_G8_B8;
            }
            if(Depth == D_32)
            {
                //Bitmask = BM_8_8_8_8;
                //Format = F_XRGB;
                PixelFormat = PF_X8_R8_G8_B8;
            }
        }
    }

    void SetDIBHeader(unsigned long int dibHeader)
    {
        if(dibHeader == BITMAPCOREHEADER)
        {DIBHeaderType = BITMAPCOREHEADER;}
        else if(dibHeader == BITMAPCOREHEADER2)
        {DIBHeaderType = BITMAPCOREHEADER2;}
        else if(dibHeader == BITMAPINFOHEADER)
        {DIBHeaderType = BITMAPINFOHEADER;}
        else if(dibHeader == BITMAPV2INFOHEADER)
        {DIBHeaderType = BITMAPV2INFOHEADER;}
        else if(dibHeader == BITMAPV3INFOHEADER)
        {DIBHeaderType = BITMAPV3INFOHEADER;}
        else if(dibHeader == BITMAPV4HEADER)
        {DIBHeaderType = BITMAPV4HEADER;}
        else if(dibHeader == BITMAPV5HEADER)
        {DIBHeaderType = BITMAPV5HEADER;}
        else
        {
            DIBHeaderType = UNSUPPORTED_HEADER;
            Error |= ERR_DIB_HEADER_UNSUPPORTED;
        }
    }

    void LoadColorTable(std::istream &in,unsigned int nColorEntries)
    {
        if(nColorEntries == 0)
        {
            return;
        }

        ColorTable = new BMColorTable(nColorEntries);

        for(unsigned int i=0;i < ColorTable->Size();i++)
        {
            char Tmp[4];

            if(DIBHeaderType == BITMAPCOREHEADER)
            {
                in.read(Tmp,3*sizeof(char));

                (*ColorTable)[i].Blue   = Tmp[0];
                (*ColorTable)[i].Green  = Tmp[1];
                (*ColorTable)[i].Red    = Tmp[2];
            }
            else
            {
                in.read(Tmp,4*sizeof(char));

                (*ColorTable)[i].Blue   = Tmp[0];
                (*ColorTable)[i].Green  = Tmp[1];
                (*ColorTable)[i].Red    = Tmp[2];
                (*ColorTable)[i].Alpha  = Tmp[3];
            }
        }
    }

    void ProcessDIBHeader(std::istream &in)
    {
        char Tmp[4];

        in.read(Tmp,4*sizeof(char));
        SetDIBHeader(Cmb_uInt32_LE(Tmp));
        if(Error & ERR_DIB_HEADER_UNSUPPORTED)
        {
            return;
        }

        if(DIBHeaderType == BITMAPCOREHEADER)
        {
            DIBHeader = new Bitmapcoreheader(in);
        }
        else if(DIBHeaderType == BITMAPCOREHEADER2)
        {
            DIBHeader = new Bitmapcoreheader2(in);
        }
        else if(DIBHeaderType == BITMAPINFOHEADER)
        {
            DIBHeader = new Bitmapinfoheader(in);
        }
        else if(DIBHeaderType == BITMAPV2INFOHEADER)
        {
            DIBHeader = new BitmapV2infoheader(in);
        }
        else if(DIBHeaderType == BITMAPV3INFOHEADER)
        {
            DIBHeader = new BitmapV3infoheader(in);
        }
        else if(DIBHeaderType == BITMAPV4HEADER)
        {
            DIBHeader = new BitmapV4header(in);
        }
        else if(DIBHeaderType == BITMAPV5HEADER)
        {
            DIBHeader = new BitmapV5header(in);
        }

        Width   = DIBHeader->GetWidth();
        Height  = DIBHeader->GetHeight();

        SetDepth(DIBHeader->GetDepth());
        if(Error & ERR_UNSUPPORTED_DEPTH)
        {
            return;
        }

        SetCompression(DIBHeader->GetCompression());
        if(Error & ERR_UNSUPPORTED_COMPRESSION)
        {
            return;
        }

        if(Depth <= D_8 && Depth != D_0)
        {
            LoadColorTable(in,DIBHeader->GetColorPalette());
        }
        SetBitmask(DIBHeader->GetRedMask(),DIBHeader->GetGreenMask(),DIBHeader->GetBlueMask(),DIBHeader->GetAlphaMask());
    }

    void LoadPixelData(std::istream &in,uint32_t StartingPoint)
    {
        if(CompressionMethod == CM_BITFIELDS || CompressionMethod == CM_RGB)
        {
            char Tmp[4];
            unsigned int RowSize = ((Width*Depth + 31) / 32)*4;
            unsigned int Padding = 4 - (RowSize % 4);
            if(Padding == 4)
            {
                Padding = 0;
            }

            PixelArray = new char[RowSize * Height * sizeof(char)];

            in.seekg(StartingPoint,in.beg);

            for(int i = Height-1;i >= 0;i--)
            {
				in.read(&PixelArray[i*RowSize],RowSize * sizeof(char));
				in.read(Tmp,Padding * sizeof(char));
            }
        }
    }

public:
    void Print(std::ostream &out)
    {
        out << filename << "\n";
        out << "Width: "<< Width << "\n";
        out << "Height: "<< Height << "\n";
        out << "Depth: "<< Depth << "\n";
        out << "Format: ";

        switch(PixelFormat)
        {
        case PF_R8_G8_B8:
            out << "R8_G8_B8\n";
            break;
        case PF_B8_G8_R8:
            out << "B8_G8_R8\n";
            break;
        case PF_LUT:
            out << "LUT\n";
            break;
            //16
            //5_6_5
        case PF_B5_G6_R5:
            out << "B5_G6_R5\n";
            break;
        case PF_R5_G6_B5:
            out << "R5_G6_B5\n";
            break;
            //4_4_4_4
        case PF_A4_B4_G4_R4:
            out << "A4_B4_G4_R4\n";
            break;
        case PF_A4_R4_G4_B4:
            out << "A4_R4_G4_B4\n";
            break;
            //1_5_5_5
        case PF_B5_G5_R5_A1:
            out << "B5_G5_R5_A1\n";
            break;
        case PF_B5_G5_R5_X1:
            out << "B5_G5_R5_X1\n";
            break;
        case PF_R5_G5_B5_A1:
            out << "R5_G5_B5_A1\n";
            break;
        case PF_R5_G5_B5_X1:
            out << "R5_G5_B5_X1\n";
            break;
            //32
            //2_10_10_10
        case PF_A2_B10_G10_R10:
            out << "A2_B10_G10_R10\n";
            break;
        case PF_A2_R10_G10_B10:
            out << "A2_R10_G10_B10\n";
            break;
            //8_8_8_8
        case PF_A8_B8_G8_R8:
            out << "A8_B8_G8_R8\n";
            break;
        case PF_X8_B8_G8_R8:
            out << "X8_B8_G8_R8\n";
            break;
        case PF_A8_R8_G8_B8:
            out << "A8_R8_G8_B8\n";
            break;
        case PF_X8_R8_G8_B8:
            out << "X8_R8_G8_B8\n";
            break;
        }

        DIBHeader->Print(out);

    }

    int GetError()
    {
        return Error;
    }

    unsigned int GetWidth()
    {
    	if(Width < 0)
		{
			return -Width;
		}
		else
		{
			return Width;
		}
    }
    unsigned int GetHeight()
    {
        return Height;
    }
    DEPTH GetDepth()
    {
        return Depth;
    }
    /*
    FORMAT GetFormat()
    {
        return Format;
    }
    BITMASK GetBitmask()
    {
        return Bitmask;
    }
    */
    PIXEL_FORMAT GetPixelFormat()
    {
        return PixelFormat;
    }
    char *GetPixelArray()
    {
        return PixelArray;
    }

    BMP(const std::string &fileName):filename(fileName),ColorTable(nullptr)
    {
        Error = 0;

        std::ifstream fin;
        fin.open(filename.c_str(),std::ifstream::binary);

        if(fin.is_open())
        {
            char Tmp[4];

            fin.read(Tmp, 2*sizeof(char));

            if(!CheckBmp(Tmp))
            {
                Error |= ERR_FILE_NOT_BMP;
                fin.close();
                return;
            }
            /*
            uint32_t Size;
            fin.read(Tmp,4*sizeof(char));
            Size = Cmb_uInt32_LE(Tmp);

            fin.seekg(4*sizeof(char),fin.cur);
            */
            fin.seekg(8*sizeof(char),fin.cur);

            uint32_t StartingPoint;
            fin.read(Tmp,4*sizeof(char));
            StartingPoint = Cmb_uInt32_LE(Tmp);

            ProcessDIBHeader(fin);
            if(Error != ERR_OK)
            {
                return;
            }

            LoadPixelData(fin,StartingPoint);

            fin.close();
        }
        else
        {
            Error |= ERR_COULDNT_OPEN_FILE;
        }
    }
    ~BMP()
    {
        delete DIBHeader;
        delete ColorTable;
        delete [] PixelArray;
    }
};

#endif
