-- GritterPanel Lua plugin

gppg = CLuaGppGraphics()

counter = 0

-- ============================================================================
-- create
function GppCreate()
end

-- ============================================================================
-- release
function GppRelease()
end

-- ============================================================================
-- start
function GppStart()
end

-- ============================================================================
-- stop
function GppStop()
end

-- ============================================================================
-- GppWorkFrame
function GppWorkFrame()
end

-- ============================================================================
-- GppDraw
function GppDraw()
	gppg:Clear()
	gppg:SetPixel( math.mod( counter, 16), math.mod(math.floor(counter/16), 16), 1 )
	gppg:SetPixel( 0, 0, 1 )
	gppg:SetPixel( 15, 15, 1 )
	gppg:SetLine( 0, 15, 15, 0, 1 )
	counter = counter + 1
end
