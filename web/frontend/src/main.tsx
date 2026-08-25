import React, { lazy, Suspense } from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import "./styles.css";

// /admin 경로만 관리자 대시보드로 — 코드 스플릿해서 고객용 번들에는
// admin.css/패널 코드가 섞이지 않는다.
const AdminApp = lazy(() => import("./admin/AdminApp"));

function Root() {
  const isAdmin = window.location.pathname.startsWith("/admin");
  if (isAdmin) {
    return (
      <Suspense fallback={null}>
        <AdminApp />
      </Suspense>
    );
  }
  return <App />;
}

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    <Root />
  </React.StrictMode>,
);
