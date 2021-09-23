/////////////////////////////////////////////////////////////////////////////
// Name:        wx/bmpbndl.h
// Purpose:     Interface of wxBitmapBundle.
// Author:      Vadim Zeitlin
// Created:     2021-09-24
// Copyright:   (c) 2021 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    Contains representations of the same bitmap in different resolutions.

    This class generalizes wxBitmap for applications supporting multiple DPIs
    and allows to operate with multiple versions of the same bitmap, in the
    sizes appropriate to the currently used display resolution, as a single
    unit. Notably, an entire wxBitmapBundle can be passed to functions such as
    wxToolBar::AddTool() to allow toolbar to select the best available bitmap
    to be shown.

    Objects of this class are typically created by the application and then
    passed to wxWidgets functions, but not used by the application itself.
    Currently bitmap bundles can be created from:

        - A vector of bitmaps, of any provenance.
        - A single wxBitmap or wxImage for backwards compatibility.

    More functions for creating bitmap bundles will be added in the future.

    Objects of wxBitmapBundle class have value-like semantics, i.e. they can be
    copied around freely (and cheaply) and don't need to be allocated on the
    heap. However they usually are created using static factory functions
    (known as "pseudo-constructors") such as FromBitmaps() instead of using the
    real constructors.

    Example of using this class to initialize a toolbar in a frame constructor:
    @code
    MyFrame::MyFrame()
        : wxFrame(nullptr, wxID_ANY, "My frame")
    {
        ...
        wxToolBar* toolBar = CreateToolBar();

        wxVector<wxBitmap> bitmaps;
        bitmaps.push_back(wxBITMAP_PNG(open_32x32));
        bitmaps.push_back(wxBITMAP_PNG(open_48x48));
        bitmaps.push_back(wxBITMAP_PNG(open_64x64));

        toolBar->AddTool(wxID_OPEN, wxBitmapBundle::FromBitmaps(bitmaps));
    }
    @endcode

    The code shown above will use 32 pixel bitmap in normal DPI, 64 pixel
    bitmap in "high DPI", i.e. pixel-doubling or 200% resolution, and 48 pixel
    bitmap in 150% resolution. For all the other resolutions, the best matching
    bitmap will be created dynamically from the best available match, e.g. for
    175% resolution, 64 pixel bitmap will be rescaled to 56 pixels.

    Of course, this code relies on actually having the resources with the
    corresponding names (i.e. @c open_NxN) in MSW .rc file or Mac application
    bundle and @c open_NxN_png arrays being defined in the program code, e.g.
    by including a file generated with @c bin2c (see wxBITMAP_PNG_FROM_DATA()),
    on the other platforms.

    Also note that the existing code using wxBitmap is compatible with the
    functions taking wxBitmapBundle in wxWidgets 3.1.6 and later because
    bitmaps are implicitly convertible to the objects of this class, so just
    passing wxBitmap to the functions taking wxBitmapBundle continues to work
    and if high resolution versions of bitmap are not (yet) available for the
    other toolbar tools, single bitmaps can continue to be used instead.

    @library{wxcore}
    @category{gdi}

    @since 3.1.6
*/
class wxBitmapBundle
{
public:
    /**
        Default constructor constructs an empty bundle.

        An empty bundle can't be used for anything, but can be assigned
        something else later.
     */
    wxBitmapBundle();

    /**
        Conversion constructor from a single bitmap.

        This constructor does the same thing as FromBitmap() and only exists
        for interoperability with the existing code using wxBitmap.
     */
    wxBitmapBundle(const wxBitmap& bitmap);

    /**
        Conversion constructor from a single image.

        Similarly to the constructor from wxBitmap, this constructor only
        exists for interoperability with the existing code using wxImage and
        can be replaced with more readable FromImage() in the new code.
     */
    wxBitmapBundle(const wxImage& image);

    /**
        Copy constructor creates a copy of another bundle.
     */
    wxBitmapBundle(const wxBitmapBundle& other);

    /**
        Assignment operator makes this bundle a copy of another bundle.
     */
    wxBitmapBundle& operator=(const wxBitmapBundle& other);


    /**
        Create a bundle from the given collection of bitmaps.

        If the @a bitmaps vector is empty, an invalid, empty bundle is
        returned, otherwise initialize the bundle with all the bitmaps in this
        vector which must be themselves valid.
     */
    static wxBitmapBundle FromBitmaps(const wxVector<wxBitmap>& bitmaps);

    /// @overload
    static wxBitmapBundle FromBitmaps(const wxBitmap& bitmap1,
                                      const wxBitmap& bitmap2);

    /**
        Create a bundle from a single bitmap.

        This is only useful for compatibility with the existing code using
        wxBitmap.

        If @a bitmap is invalid, empty bundle is returned.
     */
    static wxBitmapBundle FromBitmap(const wxBitmap& bitmap);

    /**
        Create a bundle from a single image.

        This is only useful for compatibility with the existing code using
        wxImage.

        If @a image is invalid, empty bundle is returned.
     */
    static wxBitmapBundle FromImage(const wxImage& image);

    /**
        Check if bitmap bundle is non-empty.

        Return @true if the bundle contains any bitmaps or @false if it is
        empty.
     */
    bool IsOk() const;

    /**
        Get the size of the bitmap represented by this bundle in default
        resolution or, equivalently, at 100% scaling.

        When creating the bundle from a number of bitmaps, this will be just
        the size of the smallest bitmap in it.

        Note that this function is mostly used by wxWidgets itself and not the
        application.
     */
    wxSize GetDefaultSize() const;

    /**
        Get bitmap of the specified size, creating a new bitmap from the closest
        available size by rescaling it if necessary.

        This function is mostly used by wxWidgets itself and not the
        application. As all bitmaps created by it dynamically are currently
        cached, avoid calling it for many different sizes if you do use it, as
        this will create many bitmaps that will never be deleted and will
        consume resources until the application termination.
     */
    wxBitmap GetBitmap(const wxSize size) const;
};
