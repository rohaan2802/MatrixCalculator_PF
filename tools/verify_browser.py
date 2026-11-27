"""Exercise the local presentation, calculator and gallery in installed Chrome."""
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import html
import json
from pathlib import Path
import re
from threading import Thread
from urllib.parse import unquote, urlsplit

from PIL import Image
from playwright.sync_api import sync_playwright, expect

ROOT = Path(__file__).resolve().parents[1]

class QuietHandler(SimpleHTTPRequestHandler):
    def log_message(self, format, *args):
        pass

    def copyfile(self, source, outputfile):
        try:
            super().copyfile(source, outputfile)
        except (ConnectionAbortedError, BrokenPipeError):
            pass  # A closed browser tab can cancel a lazy image request.

def verify():
    figures = json.loads((ROOT / "assets/figures/gallery.json").read_text())
    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    site = (ROOT / "index.html").read_text(encoding="utf-8")
    assert len(figures) == 24
    for figure in figures:
        for key, expected_size in (("full",(1440,900)),("preview",(960,600))):
            with Image.open(ROOT / figure[key]) as image:
                assert image.size == expected_size
            assert figure[key] in readme and figure[key] in site
        assert figure["caption"] in readme and html.escape(figure["caption"]) in site
    readme_positions = [readme.index(f["preview"]) for f in figures]
    site_positions = [site.index(f["preview"],site.index('class="gallery"')) for f in figures]
    assert readme_positions == sorted(readme_positions)
    assert site_positions == sorted(site_positions)
    results = ["24 figures, 48 dimensions, captions and README/site order verified"]
    captures = ROOT / ".test-output/visual"
    captures.mkdir(parents=True,exist_ok=True)
    with ThreadingHTTPServer(("127.0.0.1",0),partial(QuietHandler,directory=str(ROOT))) as server:
        Thread(target=server.serve_forever,daemon=True).start()
        base = f"http://127.0.0.1:{server.server_port}/"
        with sync_playwright() as p:
            browser = p.chromium.launch(channel="chrome")
            for width,height,label in ((1440,1000,"desktop"),(1024,1000,"tablet"),(390,844,"mobile"),(320,740,"narrow")):
                for theme in ("light","dark"):
                    page = browser.new_page(viewport={"width":width,"height":height},color_scheme=theme)
                    errors = []
                    page.on("pageerror",lambda error:errors.append(str(error)))
                    page.goto(base,wait_until="domcontentloaded")
                    expect(page.locator("html")).to_have_attribute("data-theme",theme)
                    assert page.evaluate("document.documentElement.scrollWidth <= innerWidth")
                    columns = page.locator(".gallery").evaluate("(el)=>getComputedStyle(el).gridTemplateColumns.split(' ').length")
                    assert columns == (3 if width>1050 else 2 if width>760 else 1)
                    page.screenshot(path=str(captures/f"{label}-{theme}.png"))
                    page.locator("#theme").click()
                    expect(page.locator("html")).to_have_attribute("data-theme","dark" if theme=="light" else "light")
                    page.locator("#theme").click()
                    if page.locator("#menu").is_visible():
                        page.locator("#menu").click()
                        expect(page.locator("#menu")).to_have_attribute("aria-expanded","true")
                        assert page.locator("#menu svg").count()==1
                        page.locator("#navigation a[href='#calculator']").click()
                        expect(page.locator("#menu")).to_have_attribute("aria-expanded","false")
                    expect(page.locator("#calculation-output")).to_contain_text("10")
                    page.locator("#operation").select_option("inverse")
                    page.locator("#calculator-form button[type='submit']").click()
                    expect(page.locator("#calculation-output")).to_contain_text("0.6")
                    with page.expect_download() as download:
                        page.locator("#export-result").click()
                    saved=json.loads(Path(download.value.path()).read_text())
                    assert saved["operation"]=="inverse"
                    assert abs(saved["result"][0][0] - .6)<1e-10
                    page.locator("#operation").select_option("multiply")
                    page.locator("#matrix-b").fill("1,2\n3,4")
                    page.locator("#calculator-form button[type='submit']").click()
                    expect(page.locator("#calculation-output")).to_contain_text("36")
                    page.locator("#operation").select_option("solve")
                    page.locator("#calculator-form button[type='submit']").click()
                    cells=page.locator("#calculation-output td").all_text_contents()
                    assert cells==["1","2"],cells
                    page.locator("#operation").select_option("inverse")
                    page.locator("#preset").select_option("singular")
                    page.locator("#calculator-form button[type='submit']").click()
                    expect(page.locator("#calculation-status")).to_contain_text("no unique")
                    expect(page.locator("#export-result")).to_be_disabled()
                    page.locator("#matrix-a").fill("nan")
                    page.locator("#calculator-form button[type='submit']").click()
                    expect(page.locator("#calculation-status")).to_contain_text("finite")
                    page.locator("#preset").select_option("tiny")
                    page.locator("#calculator-form button[type='submit']").click()
                    expect(page.locator("#calculation-status")).to_contain_text("Calculated locally")
                    page.locator("#preset").select_option("singular4")
                    page.locator("#operation").select_option("adjugate")
                    page.locator("#matrix-a").focus()
                    page.keyboard.press("Control+Enter")
                    assert page.locator("#calculation-output td").last.inner_text()=="6"
                    page.locator("#native-case").select_option("18")
                    expect(page.locator("#native-output")).to_contain_text("6.000000")
                    page.locator("#search").fill("4x4")
                    assert page.locator(".gallery figure:visible").count()==1
                    page.locator("#search").fill("no-such-figure")
                    expect(page.locator("#empty")).to_be_visible()
                    page.locator("#search").fill("")
                    page.locator("#category").select_option("Edge cases")
                    assert page.locator(".gallery figure:visible").count()==5
                    page.locator(".gallery figure:visible .capture").first.click()
                    expect(page.locator("#viewer")).to_be_visible()
                    page.wait_for_function("document.querySelector('#viewer-image').naturalWidth===1440")
                    page.locator("#next").click()
                    title=page.locator("#viewer-title").inner_text()
                    assert "system" in title
                    page.keyboard.press("ArrowLeft")
                    expect(page.locator("#viewer-title")).to_have_text("Singular inverse rejection")
                    page.locator("#previous").click()
                    expect(page.locator("#viewer-title")).to_have_text("Zero first pivot")
                    page.locator("#zoom").click()
                    expect(page.locator("#zoom")).to_have_attribute("aria-pressed","true")
                    page.locator("#zoom").click()
                    page.locator("#viewer-image").dispatch_event("touchstart",{"changedTouches":[{"identifier":0,"clientX":220,"clientY":100}]})
                    page.locator("#viewer-image").dispatch_event("touchend",{"changedTouches":[{"identifier":0,"clientX":80,"clientY":100}]})
                    expect(page.locator("#viewer-title")).to_have_text("Singular inverse rejection")
                    page.keyboard.press("Escape")
                    expect(page.locator("#viewer")).to_be_hidden()
                    page.locator("#category").select_option("All")
                    if width==320:
                        control=page.locator(".scroll-control:visible input").first
                        control.focus()
                        page.keyboard.press("End")
                        area=page.locator("#"+control.get_attribute("aria-controls"))
                        assert area.evaluate("(el)=>Math.abs(el.scrollLeft-(el.scrollWidth-el.clientWidth))<2")
                    if width==1440 and theme=="light":
                        assert page.locator("#links tbody a").count()==3
                        for target in page.locator("a[href],script[src],img[src],link[href]").evaluate_all("(els)=>els.map(el=>el.getAttribute('href')||el.getAttribute('src'))"):
                            if target.startswith(("https:","data:")):continue
                            if target.startswith("#"):
                                assert page.locator('[id="'+target[1:]+'"]').count()==1,target
                            else:
                                assert (ROOT/unquote(urlsplit(target).path)).is_file(),target
                        for link in page.locator("a[download]").all():
                            with page.expect_download() as exported:link.click()
                            assert exported.value.failure() is None
                    page.locator("#author").scroll_into_view_if_needed()
                    expect(page.locator("#back-top")).to_be_visible()
                    page.locator("#back-top").click()
                    page.wait_for_function("scrollY<10")
                    assert page.locator("#navigation .active").count()==1
                    assert not errors,errors
                    results.append(f"{width}px {theme}: calculator, validation, JSON, navigation, gallery, zoom, keys, simulated swipe and top control passed")
                    page.close()
            direct=browser.new_page(viewport={"width":390,"height":844})
            direct.goto((ROOT/"index.html").as_uri(),wait_until="domcontentloaded")
            expect(direct.locator("#calculation-output")).to_contain_text("10")
            direct.locator("#operation").select_option("inverse")
            direct.locator("#calculator-form button[type='submit']").click()
            expect(direct.locator("#calculation-output")).to_contain_text("0.6")
            results.append("Direct file preview calculates without an HTTP server")
            direct.close()
            browser.close()
        server.shutdown()
    (ROOT/"docs/reports/browser_checks.json").write_text(json.dumps(results,indent=2)+"\n",encoding="utf-8")
    print("\n".join(results))

if __name__=="__main__":
    verify()
