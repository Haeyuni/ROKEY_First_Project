from fastapi import APIRouter, Request
from sqlalchemy import text
from sqlalchemy.ext.asyncio import AsyncSession
from fastapi import Depends

from ..db import get_db
from ..schemas import HealthResponse

router = APIRouter(prefix="/api/health", tags=["health"])


@router.get("", response_model=HealthResponse)
async def health(request: Request, db: AsyncSession = Depends(get_db)) -> HealthResponse:
    ros_connected = request.app.state.ros_bridge.is_connected

    db_ok = True
    try:
        await db.execute(text("SELECT 1"))
    except Exception:
        db_ok = False

    status = "ok" if (ros_connected and db_ok) else "degraded"
    return HealthResponse(status=status, ros_connected=ros_connected, db_ok=db_ok)
