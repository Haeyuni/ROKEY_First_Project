import { useState } from "react";
import "./admin.css";
import { RecipesPanel } from "./RecipesPanel";
import { RobotStatusPanel } from "./RobotStatusPanel";
import { SessionsPanel } from "./SessionsPanel";

type Tab = "status" | "recipes" | "sessions";

export default function AdminApp() {
  const [tab, setTab] = useState<Tab>("status");

  return (
    <div className="admin-root">
      <header className="admin-header">
        <div className="admin-header__title">
          <span className="admin-header__badge">ADMIN</span>
          <h1>네일 코팅 관리자</h1>
        </div>
        <nav className="admin-tabs">
          <button
            type="button"
            className={`admin-tabs__item ${tab === "status" ? "admin-tabs__item--active" : ""}`}
            onClick={() => setTab("status")}
          >
            로봇 상태
          </button>
          <button
            type="button"
            className={`admin-tabs__item ${tab === "recipes" ? "admin-tabs__item--active" : ""}`}
            onClick={() => setTab("recipes")}
          >
            레시피
          </button>
          <button
            type="button"
            className={`admin-tabs__item ${tab === "sessions" ? "admin-tabs__item--active" : ""}`}
            onClick={() => setTab("sessions")}
          >
            세션 이력
          </button>
        </nav>
      </header>

      <main className="admin-main">
        {tab === "status" && <RobotStatusPanel />}
        {tab === "recipes" && <RecipesPanel />}
        {tab === "sessions" && <SessionsPanel />}
      </main>
    </div>
  );
}
