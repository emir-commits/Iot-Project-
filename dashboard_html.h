#ifndef DASHBOARD_HTML_H
#define DASHBOARD_HTML_H
#include <pgmspace.h>

const char DASHBOARD_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="tr">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Akıllı Sandalye Dashboard</title>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800&display=swap"
    rel="stylesheet">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box
    }

    :root {
      --bg: #0a0e1a;
      --card: rgba(255, 255, 255, 0.04);
      --border: rgba(255, 255, 255, 0.08);
      --text: #e2e8f0;
      --text2: #94a3b8;
      --accent: #6366f1;
      --accent2: #818cf8;
      --good: #10b981;
      --bad: #ef4444;
      --warn: #f59e0b;
    }

    body {
      font-family: 'Inter', sans-serif;
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
      overflow-x: hidden
    }

    body::before {
      content: '';
      position: fixed;
      top: -50%;
      left: -50%;
      width: 200%;
      height: 200%;
      background: radial-gradient(circle at 30% 20%, rgba(99, 102, 241, 0.08) 0%, transparent 50%), radial-gradient(circle at 70% 80%, rgba(16, 185, 129, 0.06) 0%, transparent 50%);
      z-index: 0
    }

    .container {
      max-width: 1200px;
      margin: 0 auto;
      padding: 20px;
      position: relative;
      z-index: 1
    }

    /* Header */
    .header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 16px 24px;
      background: var(--card);
      border: 1px solid var(--border);
      border-radius: 16px;
      margin-bottom: 24px;
      backdrop-filter: blur(20px)
    }

    .header h1 {
      font-size: 1.3rem;
      font-weight: 700;
      background: linear-gradient(135deg, var(--accent2), var(--good));
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent
    }

    .header .logo {
      font-size: 1.6rem;
      margin-right: 10px
    }

    .header-left {
      display: flex;
      align-items: center
    }

    .status-badge {
      display: flex;
      align-items: center;
      gap: 8px;
      padding: 6px 14px;
      border-radius: 20px;
      font-size: .8rem;
      font-weight: 500
    }

    .status-badge.online {
      background: rgba(16, 185, 129, 0.15);
      color: var(--good)
    }

    .status-badge.offline {
      background: rgba(239, 68, 68, 0.15);
      color: var(--bad)
    }

    .status-dot {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      animation: pulse 2s infinite
    }

    .status-badge.online .status-dot {
      background: var(--good)
    }

    .status-badge.offline .status-dot {
      background: var(--bad)
    }

    @keyframes pulse {

      0%,
      100% {
        opacity: 1
      }

      50% {
        opacity: .4
      }
    }

    /* WiFi Config */
    .wifi-bar {
      display: flex;
      gap: 10px;
      align-items: center;
      padding: 12px 20px;
      background: rgba(99, 102, 241, 0.08);
      border: 1px solid rgba(99, 102, 241, 0.2);
      border-radius: 12px;
      margin-bottom: 24px
    }

    .wifi-bar input {
      flex: 1;
      padding: 8px 14px;
      border-radius: 8px;
      border: 1px solid var(--border);
      background: var(--card);
      color: var(--text);
      font-size: .85rem;
      outline: none
    }

    .wifi-bar input:focus {
      border-color: var(--accent)
    }

    .wifi-bar button {
      padding: 8px 20px;
      border: none;
      border-radius: 8px;
      background: linear-gradient(135deg, var(--accent), #8b5cf6);
      color: #fff;
      font-weight: 600;
      cursor: pointer;
      font-size: .85rem;
      transition: transform .15s
    }

    .wifi-bar button:hover {
      transform: scale(1.03)
    }

    .wifi-bar label {
      font-size: .8rem;
      color: var(--text2);
      white-space: nowrap
    }

    /* Grid */
    .grid {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      gap: 20px;
      margin-bottom: 24px
    }

    .grid-2 {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 20px;
      margin-bottom: 24px
    }

    /* Card */
    .card {
      background: var(--card);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 24px;
      backdrop-filter: blur(20px);
      transition: border-color .3s
    }

    .card:hover {
      border-color: rgba(255, 255, 255, 0.15)
    }

    .card-title {
      font-size: .75rem;
      text-transform: uppercase;
      letter-spacing: 1.5px;
      color: var(--text2);
      margin-bottom: 16px;
      font-weight: 600
    }

    /* Gauge */
    .gauge-wrap {
      display: flex;
      flex-direction: column;
      align-items: center
    }

    .gauge {
      position: relative;
      width: 160px;
      height: 90px;
      overflow: hidden;
      margin-bottom: 8px
    }

    .gauge-bg {
      position: absolute;
      width: 160px;
      height: 160px;
      border-radius: 50%;
      border: 12px solid rgba(255, 255, 255, 0.06);
      border-bottom-color: transparent;
      border-left-color: transparent;
      transform: rotate(225deg);
      clip-path: inset(0 0 50% 0)
    }

    .gauge-fill {
      position: absolute;
      width: 160px;
      height: 160px;
      border-radius: 50%;
      border: 12px solid var(--accent);
      border-bottom-color: transparent;
      border-left-color: transparent;
      transform: rotate(225deg);
      clip-path: inset(0 0 50% 0);
      transition: transform .5s ease
    }

    .gauge-value {
      position: absolute;
      bottom: 0;
      left: 50%;
      transform: translateX(-50%);
      font-size: 1.8rem;
      font-weight: 700
    }

    .gauge-label {
      font-size: .8rem;
      color: var(--text2)
    }

    .gauge-unit {
      font-size: .9rem;
      font-weight: 400;
      color: var(--text2)
    }

    /* Posture Status */
    .posture-status {
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 180px
    }

    .posture-icon {
      font-size: 4rem;
      margin-bottom: 12px;
      transition: all .3s
    }

    .posture-text {
      font-size: 1.4rem;
      font-weight: 700;
      letter-spacing: 1px
    }

    .posture-sub {
      font-size: .8rem;
      color: var(--text2);
      margin-top: 6px
    }

    .posture-good .posture-icon {
      filter: drop-shadow(0 0 20px rgba(16, 185, 129, 0.5))
    }

    .posture-good .posture-text {
      color: var(--good)
    }

    .posture-bad .posture-icon {
      animation: shake .5s infinite
    }

    .posture-bad .posture-text {
      color: var(--bad)
    }

    @keyframes shake {

      0%,
      100% {
        transform: rotate(0)
      }

      25% {
        transform: rotate(-5deg)
      }

      75% {
        transform: rotate(5deg)
      }
    }

    /* Timer */
    .timer-display {
      font-size: 3rem;
      font-weight: 800;
      text-align: center;
      font-variant-numeric: tabular-nums;
      background: linear-gradient(135deg, var(--accent2), #c084fc);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      margin: 16px 0
    }

    /* Stats */
    .stat-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 10px 0;
      border-bottom: 1px solid var(--border)
    }

    .stat-row:last-child {
      border: none
    }

    .stat-label {
      font-size: .85rem;
      color: var(--text2)
    }

    .stat-value {
      font-size: .95rem;
      font-weight: 600
    }

    /* Chart */
    .chart-container {
      position: relative;
      height: 200px
    }

    canvas {
      width: 100% !important;
      height: 100% !important
    }

    /* Alert Bar */
    .alert-bar {
      display: none;
      padding: 14px 20px;
      border-radius: 12px;
      margin-bottom: 20px;
      font-weight: 500;
      font-size: .9rem;
      animation: slideDown .3s ease
    }

    .alert-bar.warn {
      display: flex;
      background: rgba(245, 158, 11, 0.12);
      border: 1px solid rgba(245, 158, 11, 0.3);
      color: var(--warn)
    }

    .alert-bar.danger {
      display: flex;
      background: rgba(239, 68, 68, 0.12);
      border: 1px solid rgba(239, 68, 68, 0.3);
      color: var(--bad)
    }

    @keyframes slideDown {
      from {
        opacity: 0;
        transform: translateY(-10px)
      }

      to {
        opacity: 1;
        transform: translateY(0)
      }
    }

    .alert-bar .alert-icon {
      margin-right: 10px;
      font-size: 1.1rem
    }

    /* Control Panel */
    .control-panel {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr 1fr;
      gap: 16px;
      margin-bottom: 24px
    }

    .ctrl-btn {
      padding: 14px 20px;
      border: none;
      border-radius: 12px;
      font-weight: 600;
      font-size: .85rem;
      cursor: pointer;
      transition: all .2s;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 8px
    }

    .ctrl-btn:hover {
      transform: scale(1.03)
    }

    .ctrl-btn.primary {
      background: linear-gradient(135deg, var(--accent), #8b5cf6);
      color: #fff
    }

    .ctrl-btn.success {
      background: linear-gradient(135deg, var(--good), #059669);
      color: #fff
    }

    .ctrl-btn.danger {
      background: linear-gradient(135deg, var(--bad), #dc2626);
      color: #fff
    }

    .ctrl-btn.muted {
      background: rgba(239, 68, 68, 0.15);
      color: var(--bad);
      border: 1px solid rgba(239, 68, 68, 0.3)
    }

    .slider-wrap {
      display: flex;
      flex-direction: column;
      gap: 6px
    }

    .slider-wrap label {
      font-size: .8rem;
      color: var(--text2)
    }

    .slider-wrap input[type=range] {
      width: 100%;
      accent-color: var(--accent);
      height: 6px;
      cursor: pointer
    }

    .slider-val {
      font-size: 1.1rem;
      font-weight: 700;
      color: var(--accent2);
      text-align: center
    }

    /* Responsive */
    @media(max-width:900px) {
      .grid {
        grid-template-columns: 1fr 1fr
      }

      .grid>:last-child {
        grid-column: span 2
      }

      .control-panel {
        grid-template-columns: 1fr
      }
    }

    @media(max-width:600px) {

      .grid,
      .grid-2 {
        grid-template-columns: 1fr
      }

      .grid>:last-child {
        grid-column: span 1
      }

      .timer-display {
        font-size: 2.2rem
      }

      .wifi-bar {
        flex-wrap: wrap
      }
    }
  </style>
</head>

<body>
  <div class="container">

    <!-- Header -->
    <div class="header">
      <div class="header-left">
        <span class="logo">🪑</span>
        <h1>Akıllı Sandalye Dashboard</h1>
      </div>
      <div class="status-badge offline" id="statusBadge">
        <span class="status-dot"></span>
        <span id="statusText">Bağlantı Yok</span>
      </div>
    </div>

    <!-- WiFi Config -->
    <div class="wifi-bar">
      <label>ESP32 IP:</label>
      <input type="text" id="espIp" placeholder="192.168.1.x" value="192.168.1.39">
      <button onclick="connectESP()">Bağlan</button>
    </div>

    <!-- Alert -->
    <div class="alert-bar" id="alertBar">
      <span class="alert-icon">⚠️</span>
      <span id="alertText">Duruşunuzu düzeltin!</span>
    </div>

    <!-- Main Grid -->
    <div class="grid">
      <!-- Pitch Gauge -->
      <div class="card">
        <div class="card-title">📐 Pitch Açısı</div>
        <div class="gauge-wrap">
          <div class="gauge">
            <div class="gauge-bg"></div>
            <div class="gauge-fill" id="pitchGauge"></div>
            <div class="gauge-value"><span id="pitchVal">0.0</span><span class="gauge-unit">°</span></div>
          </div>
          <div class="gauge-label">İleri / Geri Eğilme</div>
        </div>
      </div>
      <!-- Roll Gauge -->
      <div class="card">
        <div class="card-title">📐 Roll Açısı</div>
        <div class="gauge-wrap">
          <div class="gauge">
            <div class="gauge-bg"></div>
            <div class="gauge-fill" id="rollGauge"></div>
            <div class="gauge-value"><span id="rollVal">0.0</span><span class="gauge-unit">°</span></div>
          </div>
          <div class="gauge-label">Sağ / Sol Eğilme</div>
        </div>
      </div>
      <!-- Posture -->
      <div class="card">
        <div class="card-title">🧍 Duruş Durumu</div>
        <div class="posture-status posture-good" id="postureCard">
          <div class="posture-icon" id="postureIcon">🟢</div>
          <div class="posture-text" id="postureText">İYİ</div>
          <div class="posture-sub" id="postureSub">Harika gidiyorsun!</div>
        </div>
      </div>
    </div>

    <!-- Second Row -->
    <div class="grid-2">
      <!-- Timer -->
      <div class="card">
        <div class="card-title">⏱️ Oturma Süresi</div>
        <div class="timer-display" id="timerDisplay">00:00:00</div>
        <div class="stat-row">
          <span class="stat-label">Durum</span>
          <span class="stat-value" id="sitStatus">Bekleniyor</span>
        </div>
        <div class="stat-row">
          <span class="stat-label">Sistem Modu</span>
          <span class="stat-value" id="sysMode">—</span>
        </div>
      </div>
      <!-- Stats -->
      <div class="card">
        <div class="card-title">📊 Oturum İstatistikleri</div>
        <div class="stat-row">
          <span class="stat-label">Toplam Uyarı</span>
          <span class="stat-value" id="warnCount">0</span>
        </div>
        <div class="stat-row">
          <span class="stat-label">Ort. Sapma</span>
          <span class="stat-value" id="avgDev">0.0°</span>
        </div>
        <div class="stat-row">
          <span class="stat-label">Max Sapma</span>
          <span class="stat-value" id="maxDev">0.0°</span>
        </div>
        <div class="stat-row">
          <span class="stat-label">İyi Duruş Oranı</span>
          <span class="stat-value" id="goodRate">—</span>
        </div>
      </div>
    </div>

    <!-- Control Panel -->
    <div class="card">
      <div class="card-title">🎮 Uzaktan Kontrol Paneli</div>
      <div class="control-panel">
        <button class="ctrl-btn success" id="btnBuzzer" onclick="toggleBuzzer()">🔊 Oto Buzzer: Açık</button>
        <button class="ctrl-btn primary" onclick="recalibrate()">🔄 Yeniden Kalibre Et</button>
        <button class="ctrl-btn" id="btnManBuzzer" onclick="toggleManualBuzzer()"
          style="background:var(--card);border:1px solid var(--border);color:var(--text)">🚨 Manuel Buzzer</button>
        <button class="ctrl-btn" id="btnForceSit" onclick="toggleForceSit()"
          style="background:var(--card);border:1px solid var(--border);color:var(--text)">🪑 Simülasyon</button>
        <div class="card" style="padding:14px; grid-column: 1 / -1;">
          <div class="slider-wrap">
            <label>Hassasiyet (Sapma Eşiği)</label>
            <input type="range" id="threshSlider" min="5" max="30" value="10" oninput="updateThreshold(this.value)">
            <div class="slider-val">±<span id="threshVal">15</span>°</div>
          </div>
        </div>
      </div>
    </div>

    <!-- Chart -->
    <div class="card">
      <div class="card-title">📈 Açı Geçmişi (Canlı)</div>
      <div class="chart-container">
        <canvas id="chart"></canvas>
      </div>
    </div>

  </div>

  <script>
    let espIp = '';
    let polling = null;
    let connected = false;

    // Chart
    const MAX_POINTS = 100;
    let pitchHistory = [];
    let rollHistory = [];
    let timeLabels = [];

    // Stats
    let totalReadings = 0;
    let goodReadings = 0;
    let warningCount = 0;
    let devSum = 0;
    let maxDeviation = 0;

    function connectESP() {
      espIp = document.getElementById('espIp').value.trim();
      if (!espIp) return;
      if (polling) clearInterval(polling);
      fetchData();
      polling = setInterval(fetchData, 500);
    }

    async function fetchData() {
      try {
        const res = await fetch(`http://${espIp}/data`, { signal: AbortSignal.timeout(2000) });
        const d = await res.json();
        setConnected(true);
        updateUI(d);
      } catch (e) {
        setConnected(false);
      }
    }

    function setConnected(state) {
      const badge = document.getElementById('statusBadge');
      const text = document.getElementById('statusText');
      if (state && !connected) { connected = true; badge.className = 'status-badge online'; text.textContent = 'Bağlı'; }
      else if (!state && connected) { connected = false; badge.className = 'status-badge offline'; text.textContent = 'Bağlantı Yok'; }
    }

    function updateUI(d) {
      // d = { pitch, roll, refPitch, refRoll, sitting, bad, buzzer, mode, sittingMs }
      const pitch = d.pitch || 0;
      const roll = d.roll || 0;
      const isBad = d.bad || false;
      const mode = d.mode || 0; // 0=wait, 1=calib, 2=track

      manBuzzerState = d.manualBuzzer || false;
      forceSitState = d.forceSit || false;

      const bMan = document.getElementById('btnManBuzzer');
      if (manBuzzerState) {
        bMan.className = 'ctrl-btn danger';
        bMan.style.background = ''; bMan.style.border = ''; bMan.style.color = '';
        bMan.textContent = '🚨 Manuel Buzzer: AÇIK';
      } else {
        bMan.className = 'ctrl-btn';
        bMan.style.background = 'var(--card)'; bMan.style.border = '1px solid var(--border)'; bMan.style.color = 'var(--text)';
        bMan.textContent = '🚨 Manuel Buzzer';
      }

      const bForce = document.getElementById('btnForceSit');
      if (forceSitState) {
        bForce.className = 'ctrl-btn success';
        bForce.style.background = ''; bForce.style.border = ''; bForce.style.color = '';
        bForce.textContent = '🪑 Simülasyon: AKTİF';
      } else {
        bForce.className = 'ctrl-btn';
        bForce.style.background = 'var(--card)'; bForce.style.border = '1px solid var(--border)'; bForce.style.color = 'var(--text)';
        bForce.textContent = '🪑 Simülasyon';
      }

      // Angles
      document.getElementById('pitchVal').textContent = pitch.toFixed(1);
      document.getElementById('rollVal').textContent = roll.toFixed(1);

      // Gauge rotation (map -45..+45 to gauge arc)
      setGauge('pitchGauge', pitch);
      setGauge('rollGauge', roll);

      // Posture
      const card = document.getElementById('postureCard');
      const icon = document.getElementById('postureIcon');
      const txt = document.getElementById('postureText');
      const sub = document.getElementById('postureSub');
      const alert = document.getElementById('alertBar');
      const alertTxt = document.getElementById('alertText');

      if (mode < 2) {
        card.className = 'posture-status';
        icon.textContent = '⏳';
        txt.style.color = 'var(--text2)';
        txt.textContent = mode === 1 ? 'KALİBRASYON' : 'BEKLENİYOR';
        sub.textContent = mode === 1 ? 'Dik oturun, hareket etmeyin...' : 'Sandalyeye oturun';
        alert.className = 'alert-bar';
      } else if (!isBad) {
        card.className = 'posture-status posture-good';
        icon.textContent = '🟢';
        txt.textContent = 'İYİ';
        sub.textContent = 'Harika gidiyorsun!';
        alert.className = 'alert-bar';
      } else {
        card.className = 'posture-status posture-bad';
        icon.textContent = '🔴';
        txt.textContent = 'KÖTÜ';
        sub.textContent = 'Duruşunuzu düzeltin!';
        alert.className = 'alert-bar danger';
        alertTxt.textContent = '⚠️ Kötü duruş algılandı! Lütfen dik oturun.';
      }

      // Timer
      const ms = d.sittingMs || 0;
      const sec = Math.floor(ms / 1000) % 60;
      const min = Math.floor(ms / 60000) % 60;
      const hr = Math.floor(ms / 3600000);
      document.getElementById('timerDisplay').textContent =
        `${String(hr).padStart(2, '0')}:${String(min).padStart(2, '0')}:${String(sec).padStart(2, '0')}`;

      // Status texts
      document.getElementById('sitStatus').textContent = d.sitting ? 'Oturuyor' : 'Boş';
      const modes = ['Bekleme', 'Kalibrasyon', 'Takip'];
      document.getElementById('sysMode').textContent = modes[mode] || '—';

      // Stats
      if (mode === 2) {
        totalReadings++;
        const dev = Math.max(Math.abs(pitch - (d.refPitch || 0)), Math.abs(roll - (d.refRoll || 0)));
        devSum += dev;
        if (dev > maxDeviation) maxDeviation = dev;
        if (!isBad) goodReadings++;
        if (isBad && d.buzzer) {
          // Count transitions
          if (!updateUI._wasBuzzing) warningCount++;
          updateUI._wasBuzzing = true;
        } else {
          updateUI._wasBuzzing = false;
        }

        document.getElementById('warnCount').textContent = warningCount;
        document.getElementById('avgDev').textContent = (devSum / totalReadings).toFixed(1) + '°';
        document.getElementById('maxDev').textContent = maxDeviation.toFixed(1) + '°';
        document.getElementById('goodRate').textContent = ((goodReadings / totalReadings) * 100).toFixed(0) + '%';

        // Chart data
        pitchHistory.push(pitch);
        rollHistory.push(roll);
        const now = new Date();
        timeLabels.push(now.getMinutes() + ':' + String(now.getSeconds()).padStart(2, '0'));
        if (pitchHistory.length > MAX_POINTS) { pitchHistory.shift(); rollHistory.shift(); timeLabels.shift(); }
        drawChart();
      }
    }
    updateUI._wasBuzzing = false;

    function setGauge(id, angle) {
      const el = document.getElementById(id);
      const clamped = Math.max(-45, Math.min(45, angle));
      const normalized = (clamped + 45) / 90; // 0 to 1
      const rotation = 225 + (normalized * 180);
      el.style.transform = `rotate(${rotation}deg)`;
      // Color based on deviation
      const absAngle = Math.abs(angle);
      if (absAngle > 15) el.style.borderColor = `var(--bad)`;
      else if (absAngle > 10) el.style.borderColor = `var(--warn)`;
      else el.style.borderColor = `var(--good)`;
      el.style.borderBottomColor = 'transparent';
      el.style.borderLeftColor = 'transparent';
    }

    function drawChart() {
      const canvas = document.getElementById('chart');
      const ctx = canvas.getContext('2d');
      const dpr = window.devicePixelRatio || 1;
      const rect = canvas.parentElement.getBoundingClientRect();
      canvas.width = rect.width * dpr;
      canvas.height = rect.height * dpr;
      ctx.scale(dpr, dpr);
      const W = rect.width, H = rect.height;
      ctx.clearRect(0, 0, W, H);

      if (pitchHistory.length < 2) return;

      const pad = { top: 10, right: 10, bottom: 25, left: 40 };
      const cW = W - pad.left - pad.right;
      const cH = H - pad.top - pad.bottom;

      // Find range
      const allVals = [...pitchHistory, ...rollHistory];
      let minV = Math.min(...allVals, -20);
      let maxV = Math.max(...allVals, 20);
      const range = maxV - minV || 1;

      // Grid lines
      ctx.strokeStyle = 'rgba(255,255,255,0.06)';
      ctx.lineWidth = 1;
      ctx.setLineDash([4, 4]);
      for (let i = 0; i <= 4; i++) {
        const y = pad.top + (cH / 4) * i;
        ctx.beginPath(); ctx.moveTo(pad.left, y); ctx.lineTo(W - pad.right, y); ctx.stroke();
        ctx.fillStyle = 'rgba(255,255,255,0.3)';
        ctx.font = '10px Inter';
        ctx.textAlign = 'right';
        const val = maxV - (range / 4) * i;
        ctx.fillText(val.toFixed(0) + '°', pad.left - 5, y + 3);
      }
      ctx.setLineDash([]);

      // Zero line
      const zeroY = pad.top + ((maxV - 0) / range) * cH;
      ctx.strokeStyle = 'rgba(255,255,255,0.15)';
      ctx.lineWidth = 1;
      ctx.beginPath(); ctx.moveTo(pad.left, zeroY); ctx.lineTo(W - pad.right, zeroY); ctx.stroke();

      // Threshold lines (±15°)
      [15, -15].forEach(t => {
        const ty = pad.top + ((maxV - t) / range) * cH;
        ctx.strokeStyle = 'rgba(239,68,68,0.25)';
        ctx.setLineDash([6, 4]);
        ctx.beginPath(); ctx.moveTo(pad.left, ty); ctx.lineTo(W - pad.right, ty); ctx.stroke();
        ctx.setLineDash([]);
      });

      // Draw line helper
      function drawLine(data, color) {
        const gradient = ctx.createLinearGradient(0, pad.top, 0, H - pad.bottom);
        gradient.addColorStop(0, color);
        gradient.addColorStop(1, color.replace('1)', '0.3)'));

        ctx.strokeStyle = color;
        ctx.lineWidth = 2;
        ctx.lineJoin = 'round';
        ctx.beginPath();
        data.forEach((v, i) => {
          const x = pad.left + (i / (data.length - 1)) * cW;
          const y = pad.top + ((maxV - v) / range) * cH;
          i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y);
        });
        ctx.stroke();

        // Glow
        ctx.strokeStyle = color.replace('1)', '0.2)');
        ctx.lineWidth = 6;
        ctx.stroke();
      }

      drawLine(pitchHistory, 'rgba(99,102,241,1)');
      drawLine(rollHistory, 'rgba(16,185,129,1)');

      // Legend
      ctx.font = '11px Inter';
      ctx.fillStyle = 'rgba(99,102,241,1)';
      ctx.fillRect(W - 140, 8, 10, 10);
      ctx.fillStyle = 'var(--text2)';
      ctx.fillStyle = 'rgba(255,255,255,0.5)';
      ctx.textAlign = 'left';
      ctx.fillText('Pitch', W - 126, 17);
      ctx.fillStyle = 'rgba(16,185,129,1)';
      ctx.fillRect(W - 75, 8, 10, 10);
      ctx.fillStyle = 'rgba(255,255,255,0.5)';
      ctx.fillText('Roll', W - 61, 17);

      // X labels
      const step = Math.max(1, Math.floor(timeLabels.length / 6));
      ctx.fillStyle = 'rgba(255,255,255,0.3)';
      ctx.font = '9px Inter';
      ctx.textAlign = 'center';
      for (let i = 0; i < timeLabels.length; i += step) {
        const x = pad.left + (i / (timeLabels.length - 1)) * cW;
        ctx.fillText(timeLabels[i], x, H - 5);
      }
    }

    // Demo mode (ESP32 bağlı değilken test için)
    let demoMode = false;
    function startDemo() {
      demoMode = true;
      let t = 0;
      setConnected(true);
      setInterval(() => {
        t += 0.05;
        updateUI({
          pitch: Math.sin(t) * 12 + Math.random() * 3,
          roll: Math.cos(t * 0.7) * 8 + Math.random() * 2,
          refPitch: 0, refRoll: 0,
          sitting: true, bad: Math.abs(Math.sin(t) * 12) > 10,
          buzzer: Math.abs(Math.sin(t) * 12) > 15,
          mode: 2, sittingMs: t * 20000
        });
      }, 500);
    }

    let buzzerMuted = false;

    async function toggleBuzzer() {
      buzzerMuted = !buzzerMuted;
      const btn = document.getElementById('btnBuzzer');
      if (buzzerMuted) {
        btn.textContent = '🔇 Buzzer: Kapalı';
        btn.className = 'ctrl-btn muted';
      } else {
        btn.textContent = '🔊 Buzzer: Açık';
        btn.className = 'ctrl-btn success';
      }
      if (espIp) {
        try { await fetch(`http://${espIp}/control?buzzer=${buzzerMuted ? 0 : 1}`); } catch (e) { }
      }
    }

    async function recalibrate() {
      if (espIp) {
        try { await fetch(`http://${espIp}/control?recalibrate=1`); } catch (e) { }
      }
      alert('Kalibrasyon komutu gönderildi! 5 saniye dik oturun.');
    }

    let manBuzzerState = false;
    async function toggleManualBuzzer() {
      manBuzzerState = !manBuzzerState;
      if (espIp) {
        try { await fetch(`http://${espIp}/control?manual_buzzer=${manBuzzerState ? 1 : 0}`); } catch (e) { }
      }
    }

    let forceSitState = false;
    async function toggleForceSit() {
      forceSitState = !forceSitState;
      if (espIp) {
        try { await fetch(`http://${espIp}/control?force_sit=${forceSitState ? 1 : 0}`); } catch (e) { }
      }
    }

    async function updateThreshold(val) {
      document.getElementById('threshVal').textContent = val;
      if (espIp) {
        try { await fetch(`http://${espIp}/control?threshold=${val}`); } catch (e) { }
      }
    }

    // Resize handler
    window.addEventListener('resize', () => { if (pitchHistory.length) drawChart(); });
  </script>
</body>

</html>
)=====";

#endif
