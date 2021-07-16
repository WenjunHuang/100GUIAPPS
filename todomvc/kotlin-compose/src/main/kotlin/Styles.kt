import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontStyle
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.platform.Font
import androidx.compose.ui.unit.sp

object Styles {
    val PrimaryColor = Color(0xFFAF2F2F)
    val SecondaryColor = Color(0xFF2FAFAF)
    val HoverColor = Color(0xFFD15555)
    val TitleColor = Color(0x26AF2F2F)
    val BorderHintColor = Color(0xFFD0D0D0)
    val ItemNameColor = Color(0xFF272424)
    val StatusColor = Color(0xFF676363)
    val BackgroundColor = Color(0xFFE5E5E5)

    val fonts = FontFamily(
        Font("fonts/NotoSansSC-Bold.otf", weight = FontWeight.Bold),
        Font("fonts/NotoSansSC-Regular.otf", weight = FontWeight.Normal),
        Font("fonts/NotoSansSC-Regular.otf", weight = FontWeight.Normal, style = FontStyle.Italic)
    )

    val TitleTextStyle =
        TextStyle(
            fontFamily = fonts,
            fontWeight = FontWeight.Bold,
            fontSize = 72.sp,
            color = TitleColor
        )
    val ActiveItemTextStyle =
        TextStyle(
            fontFamily = fonts,
            fontWeight = FontWeight.Normal,
            fontSize = 36.sp,
            color = ItemNameColor
        )

    val CompletedItemTextStyle =
        TextStyle(
            fontFamily = fonts,
            fontWeight = FontWeight.Normal,
            fontStyle = FontStyle.Italic,
            fontSize = 36.sp,
            color = BorderHintColor
        )

    val StatusTextStyle =
        TextStyle(
            fontFamily = fonts,
            fontWeight = FontWeight.Normal,
            fontSize = 16.sp,
            color = StatusColor
        )

}
