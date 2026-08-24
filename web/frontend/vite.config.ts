import react from "@vitejs/plugin-react";
import { defineConfig } from "vite";

// NFR-05: Chrome 최신 1종만 지원 — 폴리필/레거시 타깃 불필요.
export default defineConfig({
  plugins: [react()],
  server: {
    port: 5173,
  },
});
