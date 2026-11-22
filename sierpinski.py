from counters import *
from mdims import *
from manim import *
import style

# Text.set_default(font="Cascadia Code")
Code.set_default(
    formatter_style="monokai",
    paragraph_config={"font": "Cascadia Code"}
)
def cpp_code(code: str):
    code = Code(code_string=code, language="cpp")
    code.line_numbers.set_opacity(0.3)
    return code

class Video(ThreeDScene): #ThreeDScene | MovingCameraScene
    def wait_until(self, t: float):
        self.wait(max(0, t - self.time))

    def construct(self):
        #Basic Setup
        self.camera.background_color = [12,12,12,1]

        # Extra Setup
        self.camera.frame.scale(3)

        # Create
        grid = VGroup(VGroup(Square(color=DARKER_GRAY, fill_color=DARKER_GRAY, fill_opacity=1)
                             .move_to(x * RIGHT * 2.2 + y * DOWN * 2.2) for x in range(16)) for y in range(16))
        brace = Brace(grid, direction=DOWN, buff=1, stroke_width=9)
        label = MathTex(r"16\,\mathrm{px}\;(2^4)").scale(4).next_to(brace, DOWN)

        code = cpp_code("if (x & y)").scale(3).shift(UP * 4)

        self.camera.frame.move_to(grid).save_state()

        self.play(Create(grid), run_time=2.1)
        self.play(Create(brace), Write(label), run_time=1.2)
        self.play(self.camera.frame.animate.move_to(label).scale(0.64))
        self.wait(2.4)
        self.play(self.camera.frame.animate.move_to(grid[0][0]).scale(0.5));
        self.remove(brace, label)
        self.wait(1)
        self.play(GrowFromCenter(code), run_time=0.6)
        self.wait(4)
        self.play(
            Transform(code, cpp_code("if (!(x & y))\n\timage[x,y] = {255,255,255};").scale(3).shift(UP * 4).scale(0.6)),
            run_time=0.6)
        self.wait(0.6)
        self.play(grid[0][0].animate.set_color("#AAAAAA"), run_time=0.6)
        self.wait(1.2)
        self.play(
            AnimationGroup(
                Restore(self.camera.frame),
                code.animate.scale(3.125).move_to(grid).shift(UP * 24),
                run_time=0.8
            ),
            Succession(*(pixel.animate.set_color("#AAAAAA")
                         for y, row in enumerate(grid)
                         for x, pixel in enumerate(row)
                         if not (x & y)), run_time=2.1)
        )
        self.wait(4.67)
        self.play(FadeOut(code, shift=UP), run_time=0.8)
        self.wait(5.2)
        self.play(FadeOut(grid, shift=DOWN), run_time=0.8)
        self.wait(0.33);
        code.become(cpp_code("if (x & y)").scale(9).move_to(grid).shift(UP * 14))
        self.play(FadeIn(code, shift=DOWN), run_time=0.8);
        arrow = Arrow(UP * 1, DOWN * 4, max_stroke_width_to_length_ratio=600, stroke_width=90).next_to(code,
                                                                                                       DOWN).shift(
            DOWN * 2.4);
        arrow.tip.scale(8.2)
        self.play(FadeIn(arrow, shift=DOWN), run_time=0.6);
        three_d = Text("3D?").scale(9).next_to(arrow, DOWN).shift(DOWN * 2.4)
        self.play(FadeIn(three_d, shift=DOWN), run_time=0.5)
        self.wait(1.6)
        self.play(three_d.animate.move_to(grid), FadeOut(code, arrow, shift=UP), run_time=0.6)
        self.wait(0.33);
        code.become(cpp_code("if (!(x & y | x & z | y & z)").scale(6).move_to(grid))
        self.play(Transform(three_d, code))

        # Create
        grid = VGroup(VGroup(VGroup(
            Cube(fill_color=DARKER_GRAY).move_to(RIGHT * x * 2.2 + DOWN * y * 2.2 + IN * z * 2.2).set_opacity(0.012)
            for z in range(8))
                             for y in range(8))
                      for x in range(8)
                      )
        for x in range(8):
            for y in range(8):
                grid[x][y][0].set_opacity(1)

        formula = MathTex(
            r"\begin{aligned}"
            r"A' &= A\cos(\theta)-B\sin(\theta)\\"
            r"B' &= A\sin(\theta)+B\cos(\theta)"
            r"\end{aligned}").shift(UP * 8).scale(2)

        self.play(GrowFromCenter(grid.scale(0.5).move_to(ORIGIN)))
        self.wait(2)
        self.play(Succession(*(grid[x][y][z].animate.set_color("#AAAAAA").set_opacity(1)
                               for z in range(8)
                               for y in range(8)
                               for x in range(8)
                               if not x & y | x & z | y & z), run_time=4)
                  )
        self.play(Rotate(grid, -135 * DEGREES, UP))
        self.wait(5)
        self.play(Rotate(grid, 135 * DEGREES, UP))
        self.wait(3.6)
        self.play(FadeIn(formula, shift=UP))
        self.wait(3)
        self.play(Rotate(grid, 45 * DEGREES, IN), grid.animate.scale(0.70), run_time=0.6)
        self.wait()
        self.play(Rotate(grid, 45 * DEGREES, LEFT), run_time=0.8)
        self.wait(1.6)

        self.wait(4)