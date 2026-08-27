from collections.abc import AsyncGenerator

from sqlalchemy.ext.asyncio import AsyncSession, async_sessionmaker, create_async_engine
from sqlalchemy.orm import DeclarativeBase

from .config import settings


class Base(DeclarativeBase):
    pass


engine = create_async_engine(settings.database_url, echo=False, pool_pre_ping=True)
SessionLocal = async_sessionmaker(engine, expire_on_commit=False)


async def init_db() -> None:
    """Day1 범위: 마이그레이션 도구(alembic) 없이 스키마를 직접 생성한다.

    3일짜리 개발 기간(CON-01)에 맞춘 실용적 선택. 스키마가 안정화되면
    alembic 도입을 검토한다.
    """
    async with engine.begin() as conn:
        from . import models  # noqa: F401  (모델을 Base.metadata 에 등록)

        await conn.run_sync(Base.metadata.create_all)


async def get_db() -> AsyncGenerator[AsyncSession, None]:
    async with SessionLocal() as session:
        yield session
