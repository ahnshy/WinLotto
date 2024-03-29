#include "MultiPaneCtrl.h"

struct MarkersLayoutA : DockingMarkers::Layout
{
	MarkersLayoutA() : Layout(107, 105,   // total size of marker's group.
		DockingMarkers::Marker(CPoint(0, 35), IDB_MARKER_A_LEFT, CRect(2, 2, 34, 33)),   // left marker.
		DockingMarkers::Marker(CPoint(36, 0), IDB_MARKER_A_TOP, CRect(2, 2, 33, 34)),   // top marker.
		DockingMarkers::Marker(CPoint(71, 35), IDB_MARKER_A_RIGHT, CRect(2, 2, 34, 33)),   // right marker.
		DockingMarkers::Marker(CPoint(36, 69), IDB_MARKER_A_BOTTOM, CRect(2, 2, 33, 34)),   // bottom marker.
		DockingMarkers::Marker(CPoint(15, 15), IDB_MARKER_A_CENTER, CRect(2, 2, 75, 73)),   // central marker.
		DockingMarkers::Marker(),   // without background.
		nullptr, true, CLR_NONE,   // pngs without color key.
		25, true, 14)
	{
	}
};
// 
struct MarkersLayoutB : DockingMarkers::Layout
{
	MarkersLayoutB() : Layout(88, 88,   // total size of marker's group.
		DockingMarkers::Marker(CPoint(0, 29), IDB_MARKER_B_LEFT, CRect(0, 0, 28, 29)),   // left marker.
		DockingMarkers::Marker(CPoint(29, 0), IDB_MARKER_B_TOP, CRect(0, 0, 29, 28)),   // top marker.
		DockingMarkers::Marker(CPoint(61, 29), IDB_MARKER_B_RIGHT, CRect(-1, 0, 27, 29)),   // right marker.
		DockingMarkers::Marker(CPoint(29, 60), IDB_MARKER_B_BOTTOM, CRect(0, -1, 29, 28)),   // bottom marker.
		DockingMarkers::Marker(CPoint(23, 23), IDB_MARKER_B_CENTER, CRect(5, 5, 38, 37)),   // central marker.
		DockingMarkers::Marker(CPoint(0, 0), IDB_MARKER_B_BACK),   // background.
		nullptr, true, CLR_NONE,   // pngs without color key.
		0, false, 0)
	{
	}
};
// 
struct MarkersLayoutC : DockingMarkers::Layout
{
	MarkersLayoutC() : Layout(112, 112,   // total size of marker's group.
		DockingMarkers::Marker(CPoint(4, 40), IDB_MARKER_C_LEFT, CRect(0, 0, 32, 32)),   // left marker.
		DockingMarkers::Marker(CPoint(40, 4), IDB_MARKER_C_TOP, CRect(0, 0, 32, 32)),   // top marker.
		DockingMarkers::Marker(CPoint(76, 40), IDB_MARKER_C_RIGHT, CRect(0, 0, 32, 32)),   // right marker.
		DockingMarkers::Marker(CPoint(40, 76), IDB_MARKER_C_BOTTOM, CRect(0, 0, 32, 32)),   // bottom marker.
		DockingMarkers::Marker(CPoint(40, 40), IDB_MARKER_C_CENTER, CRect(-4, -4, 36, 36)),   // central marker.
		DockingMarkers::Marker(CPoint(0, 0), IDB_MARKER_C_BACK),   // background.
		nullptr, true, CLR_NONE,   // pngs without color key.
		32, true, 14)
	{
	}
};
// 
struct MarkersLayoutD : DockingMarkers::Layout
{
	MarkersLayoutD() : Layout(112, 112,   // total size of marker's group.
		DockingMarkers::Marker(CPoint(4, 40), IDB_MARKER_D_LEFT, CRect(0, 0, 32, 32)),   // left marker.
		DockingMarkers::Marker(CPoint(40, 4), IDB_MARKER_D_TOP, CRect(0, 0, 32, 32)),   // top marker.
		DockingMarkers::Marker(CPoint(76, 40), IDB_MARKER_D_RIGHT, CRect(0, 0, 32, 32)),   // right marker.
		DockingMarkers::Marker(CPoint(40, 76), IDB_MARKER_D_BOTTOM, CRect(0, 0, 32, 32)),   // bottom marker.
		DockingMarkers::Marker(CPoint(40, 40), IDB_MARKER_D_CENTER, CRect(-4, -4, 36, 36)),   // central marker.
		DockingMarkers::Marker(CPoint(0, 0), IDB_MARKER_D_BACK),   // background.
		nullptr, true, CLR_NONE,   // pngs without color key.
		32, true, 14)
	{
	}
};
// 
struct MarkersLayoutE : DockingMarkers::Layout
{
	MarkersLayoutE() : Layout(112, 112,   // total size of marker's group.
		DockingMarkers::Marker(CPoint(4, 40), IDB_MARKER_E_LEFT, CRect(0, 0, 32, 32)),   // left marker.
		DockingMarkers::Marker(CPoint(40, 4), IDB_MARKER_E_TOP, CRect(0, 0, 32, 32)),   // top marker.
		DockingMarkers::Marker(CPoint(76, 40), IDB_MARKER_E_RIGHT, CRect(0, 0, 32, 32)),   // right marker.
		DockingMarkers::Marker(CPoint(40, 76), IDB_MARKER_E_BOTTOM, CRect(0, 0, 32, 32)),   // bottom marker.
		DockingMarkers::Marker(CPoint(40, 40), IDB_MARKER_E_CENTER, CRect(-4, -4, 36, 36)),   // central marker.
		DockingMarkers::Marker(CPoint(0, 0), IDB_MARKER_E_BACK),   // background.
		nullptr, true, CLR_NONE,   // pngs without color key.
		32, true, 16)
	{
	}
};
/////////////////////////////////////////////////////////////////////////////
// 
struct MultiPaneCtrlComplex : MultiPaneCtrl
{
	MultiPaneCtrlStyle_base styleBase;
	// 
	MultiPaneCtrlStyle_VS2003_client styleVS2003_client;
	MultiPaneCtrlStyle_VS2003_client_custom1 styleVS2003_client_custom1;
	MultiPaneCtrlStyle_VS2003_bars styleVS2003_bars;
	MultiPaneCtrlStyle_VS2003_bars_custom1 styleVS2003_bars_custom1;
	// 
	MultiPaneCtrlStyle_VS2008_client_classic styleVS2008_client_classic;
	MultiPaneCtrlStyle_VS2008_client_blue styleVS2008_client_blue;
	MultiPaneCtrlStyle_VS2008_client_silver styleVS2008_client_silver;
	MultiPaneCtrlStyle_VS2008_client_olive styleVS2008_client_olive;
	MultiPaneCtrlStyle_VS2008_bars_classic styleVS2008_bars_classic;
	MultiPaneCtrlStyle_VS2008_bars_classic_custom1 styleVS2008_bars_classic_custom1;
	MultiPaneCtrlStyle_VS2008_bars_blue styleVS2008_bars_blue;
	MultiPaneCtrlStyle_VS2008_bars_blue_custom1 styleVS2008_bars_blue_custom1;
	MultiPaneCtrlStyle_VS2008_bars_silver styleVS2008_bars_silver;
	MultiPaneCtrlStyle_VS2008_bars_silver_custom1 styleVS2008_bars_silver_custom1;
	MultiPaneCtrlStyle_VS2008_bars_olive styleVS2008_bars_olive;
	MultiPaneCtrlStyle_VS2008_bars_olive_custom1 styleVS2008_bars_olive_custom1;
	// 
	MultiPaneCtrlStyle_VS2010_client styleVS2010_client;
	MultiPaneCtrlStyle_VS2010_bars styleVS2010_bars;
	// 
	MultiPaneCtrlStyle_VS2019_client_light styleVS2019_client_light;
	MultiPaneCtrlStyle_VS2019_client_light_custom1 styleVS2019_client_light_custom1;
	MultiPaneCtrlStyle_VS2019_client_dark styleVS2019_client_dark;
	MultiPaneCtrlStyle_VS2019_client_blue styleVS2019_client_blue;
	MultiPaneCtrlStyle_VS2019_bars_light styleVS2019_bars_light;
	MultiPaneCtrlStyle_VS2019_bars_dark styleVS2019_bars_dark;
	MultiPaneCtrlStyle_VS2019_bars_blue styleVS2019_bars_blue;
};