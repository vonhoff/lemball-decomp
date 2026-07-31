class GamePasswordKeyOverlayTableView {
public:
	void RebuildSourceRects(void);
	void RebuildSourceRectsThunk(void);
};

// FUNCTION: LEMBALL 0x00451600
void GamePasswordKeyOverlayTableView::RebuildSourceRectsThunk(void)
{
	RebuildSourceRects();
}
