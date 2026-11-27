"use strict";
const icons = {
  sun: '<circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M2 12h2m16 0h2M5 5l1.5 1.5m11 11L19 19M5 19l1.5-1.5m11-11L19 5"/>',
  moon: '<path d="M20.5 13A8.5 8.5 0 0 1 11 3.5 8.5 8.5 0 1 0 20.5 13Z"/>',
  menu: '<path d="M4 6h16M4 12h16M4 18h16"/>',
  close: '<path d="m6 6 12 12M6 18 18 6"/>',
  up: '<path d="M12 20V5m-7 7 7-7 7 7"/>'
};
function icon(name) {
  return '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.7" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">' + icons[name] + '</svg>';
}
const root = document.documentElement;
const themeButton = document.querySelector("#theme");
function setTheme(theme) {
  root.dataset.theme = theme;
  themeButton.innerHTML = icon(theme === "dark" ? "sun" : "moon");
  const nextThemeLabel = theme === "dark" ? "Switch to light theme" : "Switch to dark theme";
  themeButton.setAttribute("aria-label", nextThemeLabel);
  themeButton.title = nextThemeLabel;
  try { localStorage.setItem("matrix-theme", theme); } catch {}
}
setTheme(root.dataset.theme);
themeButton.addEventListener("click", () => setTheme(root.dataset.theme === "dark" ? "light" : "dark"));
const menu = document.querySelector("#menu");
const navigation = document.querySelector("#navigation");
function closeMenu() {
  navigation.classList.remove("open");
  menu.setAttribute("aria-expanded", "false");
  menu.setAttribute("aria-label", "Open menu");
  menu.innerHTML = icon("menu");
}
closeMenu();
menu.addEventListener("click", () => {
  const open = navigation.classList.toggle("open");
  menu.setAttribute("aria-expanded", String(open));
  menu.setAttribute("aria-label", open ? "Close menu" : "Open menu");
  menu.innerHTML = icon(open ? "close" : "menu");
});
navigation.addEventListener("click", event => { if (event.target.closest("a")) closeMenu(); });
document.addEventListener("keydown", event => { if (event.key === "Escape") closeMenu(); });
const topButton = document.querySelector("#back-top");
topButton.innerHTML = icon("up");
topButton.addEventListener("click", () => window.scrollTo({top: 0}));
const navLinks = Array.from(navigation.querySelectorAll("a"));
const navSections = navLinks.map(link => document.querySelector(link.hash)).filter(Boolean).sort((first, second) => first.offsetTop - second.offsetTop);
let activeNavigationOverride = null;
function setActiveNavigation(id) {
  navLinks.forEach(link => {
    const active = link.hash === "#" + id;
    link.classList.toggle("active", active);
    if (active) link.setAttribute("aria-current", "location");
    else link.removeAttribute("aria-current");
  });
}
function updateNavigation() {
  if (activeNavigationOverride) {
    setActiveNavigation(activeNavigationOverride);
    topButton.hidden = scrollY < 600;
    return;
  }
  let current = navSections[0];
  for (const section of navSections) {
    if (section.getBoundingClientRect().top <= 160) current = section;
  }
  setActiveNavigation(current.id);
  topButton.hidden = scrollY < 600;
}
window.addEventListener("scroll", updateNavigation, {passive: true});
navigation.addEventListener("click", event => {
  const link = event.target.closest("a");
  if (link && link.hash) {
    activeNavigationOverride = link.hash.slice(1);
    setActiveNavigation(activeNavigationOverride);
    window.setTimeout(() => { activeNavigationOverride = null; }, 900);
  }
});
updateNavigation();

const scrollers = [];
document.querySelectorAll(".scroll, pre").forEach((area, index) => {
  if (!area.id) area.id = "scroll-area-" + index;
  const control = document.createElement("div");
  control.className = "scroll-control";
  const label = document.createElement("span");
  label.textContent = "Scroll horizontally";
  const slider = document.createElement("input");
  slider.type = "range";
  slider.min = "0";
  slider.max = "1000";
  slider.value = "0";
  slider.setAttribute("aria-label", "Horizontal scroll for " + area.id);
  slider.setAttribute("aria-controls", area.id);
  control.append(label, slider);
  area.after(control);
  function refresh() {
    const maximum = area.scrollWidth - area.clientWidth;
    control.hidden = maximum <= 1;
    slider.value = maximum > 0 ? String(Math.round(1000 * area.scrollLeft / maximum)) : "0";
  }
  slider.addEventListener("input", () => { area.scrollLeft = (area.scrollWidth - area.clientWidth) * Number(slider.value) / 1000; });
  area.addEventListener("scroll", refresh, {passive: true});
  scrollers.push(refresh);
});
function updateScrollers() { scrollers.forEach(refresh => refresh()); }
window.addEventListener("resize", updateScrollers);

const math = globalThis.MatrixCalculator;
const form = document.querySelector("#calculator-form");
const operation = document.querySelector("#operation");
const secondField = document.querySelector("#second-field");
const scalarField = document.querySelector("#scalar-field");
const inputA = document.querySelector("#matrix-a");
const inputB = document.querySelector("#matrix-b");
const output = document.querySelector("#calculation-output");
const status = document.querySelector("#calculation-status");
const exportButton = document.querySelector("#export-result");
let lastCalculation = null;
function invalidate() {
  lastCalculation = null;
  exportButton.disabled = true;
  output.replaceChildren();
  status.classList.remove("error");
  status.textContent = "Inputs changed. Calculate to see the result.";
  updateScrollers();
}
function syncFields() {
  secondField.hidden = !["add", "subtract", "multiply", "solve"].includes(operation.value);
  scalarField.hidden = operation.value !== "scale";
  document.querySelector("#second-label").textContent = operation.value === "solve" ? "Vector b · one value per row" : "Matrix B";
  if (operation.value === "solve") inputB.value = "18\n14";
  invalidate();
}
operation.addEventListener("change", syncFields);
form.addEventListener("input", invalidate);
const presets = {
  classic: "4, 7\n2, 6",
  pivot: "0, 2\n3, 4",
  singular: "1, 2\n2, 4",
  tiny: "1e-12, 0\n0, 2e-12",
  singular4: "1,0,0,0\n0,2,0,0\n0,0,3,0\n0,0,0,0"
};
document.querySelector("#preset").addEventListener("change", event => {
  inputA.value = presets[event.target.value];
  invalidate();
});
function formatted(value) {
  return value === 0 ? "0" : Number(value.toPrecision(12)).toString();
}
function renderResult(result) {
  output.replaceChildren();
  if (Array.isArray(result)) {
    const table = document.createElement("table");
    table.setAttribute("aria-label", "Calculated matrix result");
    const head = document.createElement("thead");
    const headings = document.createElement("tr");
    ["Row", ...result[0].map((_, index) => "Column " + (index + 1))].forEach(label => {
      const cell = document.createElement("th");
      cell.scope = "col";
      cell.textContent = label;
      headings.append(cell);
    });
    head.append(headings);
    const body = document.createElement("tbody");
    result.forEach((values, row) => {
      const tr = document.createElement("tr");
      const th = document.createElement("th");
      th.scope = "row";
      th.textContent = "Row " + (row + 1);
      tr.append(th);
      for (const value of values) {
        const cell = document.createElement("td");
        cell.textContent = formatted(value);
        tr.append(cell);
      }
      body.append(tr);
    });
    table.append(head, body);
    output.append(table);
  } else {
    const element = document.createElement("p");
    element.className = "scalar";
    element.textContent = typeof result === "number" ? formatted(result) : JSON.stringify(result, null, 2);
    output.append(element);
  }
}
form.addEventListener("submit", event => {
  event.preventDefault();
  try {
    const matrix = math.parseMatrix(inputA.value);
    let second = null;
    if (!secondField.hidden) {
      if (operation.value === "solve") {
        second = inputB.value.trim().split(/[\s,;]+/).map(token => {
          const value = Number(token);
          if (!token || !Number.isFinite(value)) throw new Error("Vector entries must be finite numbers.");
          return [value];
        });
      } else second = math.parseMatrix(inputB.value);
    }
    const scalarText = document.querySelector("#scalar").value.trim();
    if (operation.value === "scale" && !scalarText) throw new Error("Enter a finite scalar.");
    const scalar = Number(scalarText);
    const result = math.calculate(operation.value, matrix, second, scalar);
    lastCalculation = {operation: operation.value, matrix, second, scalar: operation.value === "scale" ? scalar : null, result};
    renderResult(result);
    status.classList.remove("error");
    status.textContent = "Calculated locally in the browser.";
    exportButton.disabled = false;
  } catch (error) {
    output.replaceChildren();
    lastCalculation = null;
    exportButton.disabled = true;
    status.classList.add("error");
    status.textContent = error.message;
  }
  updateScrollers();
});
form.addEventListener("keydown", event => {
  if (event.ctrlKey && event.key === "Enter") { event.preventDefault(); form.requestSubmit(); }
});
exportButton.addEventListener("click", () => {
  if (!lastCalculation) return;
  const url = URL.createObjectURL(new Blob([JSON.stringify(lastCalculation, null, 2) + "\n"], {type: "application/json"}));
  const link = document.createElement("a");
  link.href = url;
  link.download = "matrix-result.json";
  link.click();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
});
operation.value = "determinant";
syncFields();
form.requestSubmit();

const records = JSON.parse(document.querySelector("#native-data").textContent);
function showNativeCase() {
  const record = records[Number(document.querySelector("#native-case").value)];
  document.querySelector("#native-output").textContent = record.focus;
  document.querySelector("#native-transcript").href = record.transcript;
  updateScrollers();
}
document.querySelector("#native-case").addEventListener("change", showNativeCase);
showNativeCase();

const figures = JSON.parse(document.querySelector("#figure-data").textContent);
const cards = Array.from(document.querySelectorAll(".gallery figure"));
const search = document.querySelector("#search");
const category = document.querySelector("#category");
let visible = figures.map((_, index) => index);
function filterFigures() {
  visible = [];
  const term = search.value.trim().toLowerCase();
  cards.forEach((card, index) => {
    const figure = figures[index];
    const matches = (category.value === "All" || category.value === figure.category) && (figure.title + " " + figure.caption).toLowerCase().includes(term);
    card.hidden = !matches;
    if (matches) visible.push(index);
  });
  document.querySelector("#count").textContent = visible.length + " of 24 figures";
  document.querySelector("#empty").hidden = visible.length > 0;
}
search.addEventListener("input", filterFigures);
category.addEventListener("change", filterFigures);
filterFigures();

const viewer = document.querySelector("#viewer");
const viewerImage = document.querySelector("#viewer-image");
const zoomButton = document.querySelector("#zoom");
document.querySelector("#close").innerHTML = icon("close");
let selected = 0;
function showFigure(index) {
  selected = index;
  const figure = figures[index];
  viewerImage.src = figure.full;
  viewerImage.alt = figure.title;
  document.querySelector("#viewer-title").textContent = figure.title;
  document.querySelector("#viewer-caption").textContent = figure.caption;
  document.querySelector("#original").href = figure.full;
  viewerImage.parentElement.classList.remove("zoomed");
  zoomButton.setAttribute("aria-pressed", "false");
}
function moveFigure(delta) {
  const position = visible.indexOf(selected);
  showFigure(visible[(position + delta + visible.length) % visible.length]);
}
document.querySelector(".gallery").addEventListener("click", event => {
  const button = event.target.closest(".capture");
  if (!button) return;
  showFigure(Number(button.dataset.index));
  viewer.showModal();
  document.body.classList.add("modal-open");
});
document.querySelector("#close").addEventListener("click", () => viewer.close());
viewer.addEventListener("close", () => document.body.classList.remove("modal-open"));
document.querySelector("#next").addEventListener("click", () => moveFigure(1));
document.querySelector("#previous").addEventListener("click", () => moveFigure(-1));
viewer.addEventListener("keydown", event => {
  if (event.key === "ArrowRight") { event.preventDefault(); moveFigure(1); }
  if (event.key === "ArrowLeft") { event.preventDefault(); moveFigure(-1); }
});
zoomButton.addEventListener("click", () => {
  const zoomed = viewerImage.parentElement.classList.toggle("zoomed");
  zoomButton.setAttribute("aria-pressed", String(zoomed));
});
let touchX = 0;
let touchY = 0;
viewerImage.addEventListener("touchstart", event => {
  touchX = event.changedTouches[0].clientX;
  touchY = event.changedTouches[0].clientY;
}, {passive: true});
viewerImage.addEventListener("touchend", event => {
  if (zoomButton.getAttribute("aria-pressed") === "true") return;
  const dx = event.changedTouches[0].clientX - touchX;
  const dy = event.changedTouches[0].clientY - touchY;
  if (Math.abs(dx) > 60 && Math.abs(dx) > Math.abs(dy)) moveFigure(dx < 0 ? 1 : -1);
}, {passive: true});
updateScrollers();
